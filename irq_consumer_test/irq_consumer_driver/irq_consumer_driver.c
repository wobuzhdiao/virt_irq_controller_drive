#include "irq_consumer_driver.h"
/*virt_irq_provider_t*/

static irqreturn_t irq_consumer_of_led_handler(int irq, void *data)
{
	printk("%s:%d irq=%d\n", __FUNCTION__, __LINE__, irq);	

	return IRQ_HANDLED;
}

static irqreturn_t irq_consumer_of_key_handler(int irq, void *data)
{
	printk("%s:%d irq=%d\n", __FUNCTION__, __LINE__, irq);	

	return IRQ_HANDLED;
}

static int irq_consumer_probe(struct platform_device *platform_dev)
{
	struct irq_consumer_info *infop  = platform_dev->dev.platform_data;
	int ret = 0;
	struct irq_consumer_dev *devp = NULL;

	
	if(infop == NULL)
		return -EINVAL;

	devp = devm_kzalloc(&platform_dev->dev, sizeof(struct irq_consumer_dev), GFP_KERNEL);
	if(devp == NULL)
		return -ENOMEM;
	
	devp->irq_index_for_virt0808 = infop->irq_index_for_virt0808;
	devp->irq_index_for_virt0813 = infop->irq_index_for_virt0813;
	ret = devm_request_irq(&platform_dev->dev, devp->irq_index_for_virt0808, irq_consumer_of_key_handler, 0,
							platform_dev->name, devp);
	if (ret < 0) 
	{
		printk("request_irq() for %d failed: %d", devp->irq_index_for_virt0808, ret);
		return ret;
	}
	irq_set_irq_type(devp->irq_index_for_virt0808, IRQ_TYPE_EDGE_RISING);
	ret = devm_request_irq(&platform_dev->dev, devp->irq_index_for_virt0813, irq_consumer_of_led_handler, 0,
							platform_dev->name, devp);
	if (ret < 0) 
	{
		printk("request_irq() for %d failed: %d", devp->irq_index_for_virt0813, ret);
		return ret;
	}
	irq_set_irq_type(devp->irq_index_for_virt0813, IRQ_TYPE_EDGE_RISING);

	platform_set_drvdata(platform_dev, devp);
	printk("%s:%d\n", __FUNCTION__, __LINE__);
	return 0;
} 

static int irq_consumer_remove(struct platform_device *platform_dev)
{
	printk("%s:%d\n", __FUNCTION__, __LINE__);
	platform_set_drvdata(platform_dev, NULL);

	return 0;
}


static struct platform_driver irq_consumer_platform_driver = {
	.driver = {
		.name = "virt_irq_consumer",
		.owner = THIS_MODULE,
	},
	.probe = irq_consumer_probe,
	.remove = irq_consumer_remove,
};


static int __init irq_consumer_platform_driver_init(void)
{
	
	return platform_driver_register(&irq_consumer_platform_driver);
}

static void __exit irq_consumer_platform_driver_exit(void)
{
	platform_driver_unregister(&irq_consumer_platform_driver);
}

module_init(irq_consumer_platform_driver_init);
module_exit(irq_consumer_platform_driver_exit);
MODULE_DESCRIPTION("Irq Consumer Device Driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("jerry_chg");
