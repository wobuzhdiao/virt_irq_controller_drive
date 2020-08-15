#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mdio.h>
#include <linux/phy.h>
#include <linux/platform_device.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/kdev_t.h>
#include <linux/syscalls.h>
#include <linux/mount.h>
#include <linux/device.h>
#include "../irq_consumer_driver/irq_consumer_driver.h"

static void irq_consumer_dev_release(struct device *dev)
{

}


static struct irq_consumer_info consumer_info =
{
	.irq_index_for_virt0813 = 96,
	.irq_index_for_virt0808 = 86,
};
static struct platform_device irq_consumer_devplatform_device = {
	.name = "virt_irq_consumer",
	.id = 0,
	.dev =
	{
		.release = irq_consumer_dev_release, 
		.platform_data = &consumer_info,
	}
};

static int __init irq_consumer_platform_dev_init(void)
{
	platform_device_register(&irq_consumer_devplatform_device);

	return 0;
}

static void __exit irq_consumer_platform_dev_exit(void)
{
	platform_device_unregister(&irq_consumer_devplatform_device);
}


module_init(irq_consumer_platform_dev_init);
module_exit(irq_consumer_platform_dev_exit);

MODULE_DESCRIPTION("Irq Consumer Devs");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("jerry_chg");
