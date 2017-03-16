ifneq ($(KERNELRELEASE),)
obj-m := sysfs_example.o
else
KDIR := ../../../Linux/linux-4.0.2

all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif
