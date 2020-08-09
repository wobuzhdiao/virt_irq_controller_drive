BUILD_DIRS = irq_consumer_dev  irq_consumer_driver 

all:
	@$(foreach dir, $(BUILD_DIRS),  $(MAKE) -C $(dir) || exit 1;)

install:
	@$(foreach dir, $(BUILD_DIRS),  $(MAKE) -C $(dir) install || exit 1;)

clean:
	@$(foreach dir, $(BUILD_DIRS),  $(MAKE) -C $(dir) clean || exit 1;)
	rm -rf $(IMAGES_DIR)

.PHONY:all install clean
