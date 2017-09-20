#	File Description : Use of cmock framework to make unit tests for cicular buffer and linked list
#	File Author	 : Vihanga Bare
#	Assignment Date  : 9/18/2017
#	Tools used       : Ubuntu 16.04.2, GCC version 5.4.0, cmocka, cmake	


## FILE STRUCTURE

3rd party - cmocka is included in this directory. See 3rd-party/README.md for details.
Sources - circbuff.c - Source code for circular buffer
	  doublell.c - Source code for DLL 
	  test_circuff.c - Source code for unit tests for circular buffer
          test_doublell.c - Source code for unit tests for double linked list

Headers - circbuff.h - Header files for circular buffer
	  doublell.h - Header files for double linked list

## Requirements

install cmake - sudo apt-get install cmake

## Building cmocka: $ make cmocka

This will go into the 3rd-party libraries directory and call cmake correctly to build cmocka.

## To run test:

for double linked list

$ make test TEST=doublell 

for circular buffer 

$ make test TEST=circular 
