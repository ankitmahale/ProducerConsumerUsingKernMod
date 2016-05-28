obj-m += myPipe.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

producer2:producer.c
	gcc -o producer producer.c

consumer2:consumer.c
	gcc -o consumer consumer.c
