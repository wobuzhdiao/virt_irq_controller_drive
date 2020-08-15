KERNEL_SOURCE=/lib/modules/$(shell uname -r)/build
CURR_DIR=$(shell pwd)
OBJ_NAME=virt0813_irq
TARGET=$(OBJ_NAME).ko
obj-m += $(OBJ_NAME).o


all:
	$(MAKE) -C $(KERNEL_SOURCE) M=$(CURR_DIR) modules

install:
	$(HOST_INSTALL) $(TARGET) $(IMAGES_DIR)
clean:
	$(MAKE) -C $(KERNEL_SOURCE) M=$(CURR_DIR) clean

.PHONY:all install clean
