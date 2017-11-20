obj-m+=char_module.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(shell pwd) modules
	$(CC) user_char_module.c -o socket_test.o
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(shell pwd) clean
	rm socket_test.o
