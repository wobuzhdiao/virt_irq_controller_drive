#include "virt0808_irq.h"


static ssize_t virt_irq_info_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct virt_irq_provider *devp = dev_get_drvdata(dev);



	return  sprintf(buf, "mask reg value:0x%x\nlevel reg value:0x%x \nedge reg value:0x%x\n",
		devp->irq_mask_reg, devp->irq_level_reg, devp->irq_edge_reg);
}

static ssize_t virt_irq_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct virt_irq_provider *devp = dev_get_drvdata(dev);
	u32 trigger_value = simple_strtoul(buf, NULL, 10);

	if(trigger_value >= devp->irq_num)
		return -EINVAL;

	printk("%s:%d trigger irq=%d\n", __FUNCTION__, __LINE__, trigger_value);
	devp->irq_trigger_value = trigger_value + devp->irq_base;

	irq_work_queue(&devp->work);
	printk("%s:%d trigger irq=%d\n", __FUNCTION__, __LINE__, trigger_value);
	return count;
}

static DEVICE_ATTR(irq_trigger, S_IWUSR|S_IRUSR, NULL, virt_irq_store);
static DEVICE_ATTR(irq_info, S_IRUSR, virt_irq_info_show, NULL);


static struct attribute *virt_irq_chip_attrs[] =
{
	&dev_attr_irq_trigger.attr,
	&dev_attr_irq_info.attr,
	NULL
};
static const struct attribute_group virt_irq_chip_attr_group = 
{
	.attrs = virt_irq_chip_attrs,
};

void virt_irq_unmask(struct irq_data *data)
{
	struct irq_chip *chip = irq_data_get_irq_chip(data);
	struct virt_irq_provider *devp = container_of(chip, struct virt_irq_provider, irq_chip);
	uint8_t irq_index = data->irq - devp->irq_base;
	printk("%s:%d irq_num=%d\n", __FUNCTION__, __LINE__, data->irq);

	spin_lock(&devp->lock);
	devp->irq_mask_reg &= ~(1<<irq_index);
	spin_unlock(&devp->lock);
}

void virt_irq_mask(struct irq_data *data)
{
	struct irq_chip *chip = irq_data_get_irq_chip(data);
	struct virt_irq_provider *devp = container_of(chip, struct virt_irq_provider, irq_chip);
	uint8_t irq_index = data->irq - devp->irq_base;
	printk("%s:%d irq_num=%d\n", __FUNCTION__, __LINE__, data->irq);

	spin_lock(&devp->lock);
	devp->irq_mask_reg |= (1<<irq_index);
	spin_unlock(&devp->lock);

}

int virt_irq_set_affinity(struct irq_data *data, const struct cpumask *dest, bool force)
{
	/*set affinity*/
	return 0;
}

int  virt_irq_set_type(struct irq_data *data, unsigned int flow_type)
{
	struct irq_chip *chip = irq_data_get_irq_chip(data);
	struct virt_irq_provider *devp = container_of(chip, struct virt_irq_provider, irq_chip);
	uint8_t bit_shift = data->irq - devp->irq_base;
	uint32_t level = 0;
	uint32_t edge = 0;
	int ret = 0;

	printk("%s:%d irq_num=%d\n", __FUNCTION__, __LINE__, bit_shift);
	spin_lock(&devp->lock);
	level = devp->irq_level_reg;
	edge = devp->irq_edge_reg;
	if (flow_type == IRQ_TYPE_EDGE_RISING) {
		level |= (1<<bit_shift);
		edge |= (1<<bit_shift);
	} else if (flow_type == IRQ_TYPE_EDGE_FALLING) {
		level &= ~(1<<bit_shift);
		edge |= (1<<bit_shift);
	} else if (flow_type == IRQ_TYPE_LEVEL_LOW) {
		level &= ~(1<<bit_shift);
		edge &= ~(1<<bit_shift);
	} else if (flow_type == IRQ_TYPE_LEVEL_HIGH) {
		level |= (1<<bit_shift);
		edge &= ~(1<<bit_shift);
	} else if(flow_type == IRQ_TYPE_NONE)
	{
		//low type
		level &= ~(1<<bit_shift);
		edge &= ~(1<<bit_shift);
	}
	else
	{
		ret = -1;
		goto unlock;
	}

	
	devp->irq_level_reg = level;
	devp->irq_edge_reg = edge;
unlock:
	spin_unlock(&devp->lock);

	return ret;
}

static int virt_irq_init(struct virt_irq_provider *devp)
{
	int i = 0;
	int ret = 0;


	ret = irq_alloc_descs(devp->irq_base, 0, devp->irq_num, 0);
	for(i = devp->irq_base; i < (devp->irq_base+devp->irq_num); i++)
	{
		ret = irq_set_chip(i, &devp->irq_chip);
		irq_set_handler(i, handle_level_irq);		
	}
	
	return 0;
}


static void virt_irq_trigger_func(struct irq_work * workp)
{
	struct virt_irq_provider *devp = container_of(workp, struct virt_irq_provider, work);
	int ret = 0;

	printk("%s:%d\n", __FUNCTION__, __LINE__);
	ret = generic_handle_irq(devp->irq_trigger_value);

	if(ret != 0)
	{
		printk("%s:%d err=%d\n", __FUNCTION__, __LINE__, ret);
	}
	printk("%s:%d\n", __FUNCTION__, __LINE__);
}

static int virt_irq_platform_probe(struct platform_device *platform_dev)
{
	struct virt_irq_controller_data *datap = platform_dev->dev.platform_data;
	struct virt_irq_provider *devp = devm_kzalloc(&platform_dev->dev, sizeof(struct virt_irq_provider), GFP_KERNEL);

	if(devp == NULL)
		return -ENOMEM;

	devp->irq_chip.irq_unmask= virt_irq_unmask;
	devp->irq_chip.irq_mask = virt_irq_mask;
	devp->irq_chip.irq_set_type = virt_irq_set_type;
	devp->irq_chip.irq_set_affinity = virt_irq_set_affinity;
	devp->irq_base = datap->irq_base;
	devp->irq_num = datap->irq_num;
	spin_lock_init(&devp->lock);
	virt_irq_init(devp);
	init_irq_work(&devp->work, virt_irq_trigger_func);
	printk("%s:%d\n", __FUNCTION__, __LINE__);
	sysfs_create_group(&(platform_dev->dev.kobj), &virt_irq_chip_attr_group);
	platform_set_drvdata(platform_dev, devp);
	printk("%s:%d\n", __FUNCTION__, __LINE__);
	return 0;
} 

static int virt_irq_platform_remove(struct platform_device *platform_dev)
{

	struct virt_irq_provider *devp = platform_get_drvdata(platform_dev);

	printk("%s:%d\n", __FUNCTION__, __LINE__);

	irq_free_descs(devp->irq_base, devp->irq_num);
	sysfs_remove_group(&(platform_dev->dev.kobj), &virt_irq_chip_attr_group);
	platform_set_drvdata(platform_dev, NULL);

	return 0;
}


static struct platform_driver virt_irq_platform_driver = {
	.driver = {
		.name = "virt0808_irq_dev",
		.owner = THIS_MODULE,
	},
	.probe = virt_irq_platform_probe,
	.remove = virt_irq_platform_remove,
};


static int __init virt_irq_device_init(void)
{
	int ret = 0;
	
	ret = platform_driver_register(&virt_irq_platform_driver);

	return ret;
}

static void __exit virt_irq_device_exit(void)
{
	platform_driver_unregister(&virt_irq_platform_driver);
}

module_init(virt_irq_device_init);
module_exit(virt_irq_device_exit);
MODULE_DESCRIPTION("Virtual IRQ Device");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("jerry_chg");
