ifneq ($(KERNELRELEASE),)
obj-m := hello.o
else
KDIR := ../../../Linux/linux-4.0.2

all:
	$(MAKE) -C $(KDIR) M=$$PWD
endif
