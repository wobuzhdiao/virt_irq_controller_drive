#ifndef VIRT0808_IRQ_H_
#define VIRT0808_IRQ_H_

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
#include <linux/irqdomain.h>


struct virt_irq_controller_data
{
	int irq_base;
	int irq_num;
};

struct virt_irq_provider
{
	int irq_trigger_value;
	struct irq_work work;
	int irq_base;
	int irq_num;

	uint32_t irq_mask_reg;
	uint32_t irq_level_reg;
	uint32_t irq_edge_reg;
	struct irq_chip irq_chip;
	struct irq_domain	*irq_domain;
	struct platform_device *platform_dev;
	spinlock_t lock;
};

#endif
