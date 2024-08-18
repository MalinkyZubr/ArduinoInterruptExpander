# -*- MakeFile -*-

INC_PATH=-I./include -I/home/malinkyzubr/.arduino15/packages/arduino/hardware/avr/1.8.6/variants/standard -I/usr/lib/avr/include/ -I/home/malinkyzubr/.arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino -I/home/malinkyzubr/.arduino15/packages/arduino/hardware/avr/1.8.6/libraries -I/home/malinkyzubr/.arduino15/packages/arduino/hardware/avr/1.8.6/libraries/SPI/src -I/home/malinkyzubr/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/avr/include/avr/

LIB_PATH=./lib
LIB_NAME=libinterruptexpander.a
SRC_PATH=./src
OBJ_PATH=./src/obj
EXAMPLES_PATH=./examples
EXAMPLES_OBJECTS_PATH=$(EXAMPLES_PATH)/objects
EXAMPLES_HEX_PATH=$(EXAMPLES_PATH)/hex
CC=avr-gcc
CFLAGS=-g -O2 -mmcu=atmega328p -D F_CPU=16000000UL

C_SOURCES=$(wildcard ./src/*.c) 
CPP_SOURCES=$(wildcard ./src/*.cpp)
C_OBJECTS=$(patsubst $(SRC_PATH)/%.c, $(OBJ_PATH)/%.o, $(C_SOURCES))
CPP_OBJECTS=$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(CPP_SOURCES))


all: LIB_ARCHIVE INTERFACE_BINARY

INTERFACE_BINARY: LIB_ARCHIVE 
	mkdir -p $(EXAMPLES_OBJECTS_PATH)
	mkdir -p $(EXAMPLES_HEX_PATH)
	$(CC) $(CFLAGS) -c -o $(EXAMPLES_OBJECTS_PATH)/interrupt_test.o $(EXAMPLES_PATH)/interrupt_test.cpp $(INC_PATH)
	$(CC) $(CFLAGS) -o  $(EXAMPLES_OBJECTS_PATH)/interrupt_test.elf $(EXAMPLES_OBJECTS_PATH)/interrupt_test.o -L$(LIB_PATH) -linterruptexpander
	avr-objcopy -O ihex -R .eeprom $(EXAMPLES_OBJECTS_PATH)/interrupt_test.elf $(EXAMPLES_HEX_PATH)/test.hex

$(LIB_PATH):
	mkdir -p $(LIB_PATH)
	ar -r $(LIB_PATH)/$(LIB_NAME)

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

LIB_ARCHIVE: $(CPP_OBJECTS) $(C_OBJECTS) $(LIB_PATH)
	echo $(CPP_OBJECTS)
	ar -rvs $(LIB_PATH)/$(LIB_NAME) $(CPP_OBJECTS)
	ar -rvs $(LIB_PATH)/$(LIB_NAME) $(C_OBJECTS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c* | $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC_PATH)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC_PATH)

clean: 
	rm -rf $(LIB_PATH)
	rm -rf $(OBJ_PATH)
	rm -rf $(EXAMPLES_OBJECTS_PATH)
	rm -rf $(EXAMPLES_HEX_PATH)
