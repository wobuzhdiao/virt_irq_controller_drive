#ifndef VIRTUAL_SPI_CONTROLLER_H_
#define VIRTUAL_SPI_CONTROLLER_H_

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
#include <linux/interrupt.h>
#include <linux/syscalls.h>
#include <linux/mount.h>
#include <linux/device.h>
#include <linux/genhd.h>
#include <linux/namei.h>
#include <linux/shmem_fs.h>
#include <linux/ramfs.h>
#include <linux/sched.h>
#include <linux/vfs.h>
#include <linux/pagemap.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/dcache.h>
#include <linux/irq.h>
#include <linux/irq_work.h>

struct irq_consumer_info
{
	int irq_index;
};


struct irq_consumer_dev
{
	int irq_index;
	/*priavte data for consumer device driver....*/
};

#endif
