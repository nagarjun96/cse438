TOOLDIR = /opt/iot-devkit/1.7.3/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux
ARCH = x86
CROSS_COMPILE=/opt/iot-devkit/1.7.3/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux/i586-poky-linux-

ifeq ($(TEST_TARGET), Galileo2)
	CC=$(TOOLDIR)/i586-poky-linux-gcc
	MAKE=make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)
	KERN_DIR:=/opt/iot-devkit/1.7.3/sysroots/i586-poky-linux/usr/src/kernel
else
	CC=gcc
	MAKE=make
	KERN_DIR=/lib/modules/$(shell uname -r)/build
endif

obj-m+=squeue.o
 
all:
	make -C $(KERN_DIR) M=$(PWD) modules

clean:
	make -C $(KERN_DIR) M=$(PWD) clean
	rm test
