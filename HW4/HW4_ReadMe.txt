[Problem 1 - 30 pts] (Character driver - BBG) is in source code file named char_module.c
[Problem 2 - 10 pts] (User space application - BBG) is in source code file user_char_module.c
[Problem 3 - 20 pts] (IPC) server side source code is in file user_char_module.c, client side source code is in file socket_client.c
[Problem 4 - 15 pts] Project 2 Writeup is in file APES_P2.pdf


Steps to run the character driver (for successfully running)- 
1. Unbind the existing led driver on Beaglebone green, do this as root - echo leds > /sys/class/leds/beaglebone\:green\:usr0/device/driver/unbind
2. Refer to the Makefile uploaded, do sudo make all, to compile the LKM for character driver
3. insert the kernel module - insmod ./char_module.ko
4. check the /var/log/syslog file for major number
5. remove any existing char device named /dev/SimplecharDrv by sudo rm /dev/SimplecharDrv
6. use major number in step 4, and run command sudo mknod -m 666 /dev/SimplecharDrv c <majornumber> 0
7. Now the device has been created so we can begin file operations,
8. run the server side socket application user_char_module.c, which also acts as user space code that reads, writes, to the driver by 
gcc user_char_module.c -o socket_test.o
./socket_test.o <portnum>

9. run the client side socket application socket_client.c, which sends commands to server side, by
gcc socket_client.c -o socket_client.o
./socket_client.o 192.168.7.2 <portnum>



