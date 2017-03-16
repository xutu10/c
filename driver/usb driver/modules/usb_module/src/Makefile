ifneq ($(KERNELRELEASE),)
obj-m := usb_module.o
else
KDIR := /lib/modules/$(shell uname -r)/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD modules
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
	rm *.o *.ko
endif

