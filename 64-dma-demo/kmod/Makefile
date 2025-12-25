obj-m += dma_read_demo.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

insert:
	sudo insmod dma_read_demo.ko

remove:
	sudo modprobe -r dma_read_demo.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
