KERNEL_SRC := /lib/modules/$(shell uname -r)/build
MAKE := make

obj-m := userv4l2.o
userv4l2-objs := file.o common.o v4l2.o ioreq.o

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules_clean