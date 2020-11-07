KERNEL=$(shell uname -r)

obj-m = hid-kbd-custom.o

all:
	make -C /lib/modules/${KERNEL}/build M=$(PWD) modules

clean:
	make -C /lib/modules/${KERNEL}/build M=$(PWD) clean
