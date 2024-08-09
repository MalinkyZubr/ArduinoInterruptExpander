# -*- MakeFile -*-

INC_PATH=-I./include -I/home/malinkyzubr/.arduino15/packages/arduino/hardware/avr/1.8.6/variants/standard -I/usr/lib/avr/include/ -I/home/malinkyzubr/.arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino -I/home/malinkyzubr/.arduino15/packages/arduino/hardware/avr/1.8.6/libraries -I/home/malinkyzubr/.arduino15/packages/arduino/hardware/avr/1.8.6/libraries/SPI/src -I/home/malinkyzubr/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/avr/include/avr/

LIB_PATH=./lib
SRC_PATH=./src
OBJ_PATH=./src/obj
CC=avr-gcc
CFLAGS=-g -O2 -mmcu=atmega328p -D F_CPU=16000000UL

C_SOURCES=$(wildcard ./src/*.c) 
CPP_SOURCES=$(wildcard ./src/*.cpp)
C_OBJECTS=$(patsubst $(SRC_PATH)/%.c, $(OBJ_PATH)/%.o, $(C_SOURCES))
CPP_OBJECTS=$(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(CPP_SOURCES))


all: LIB_ARCHIVE

$(LIB_PATH):
	mkdir -p $(LIB_PATH)
	ar -r $(LIB_PATH)/ArduinoInterruptExpander.a

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

LIB_ARCHIVE: $(CPP_OBJECTS) $(C_OBJECTS) $(LIB_PATH)
	echo $(CPP_OBJECTS)
	ar -rvs $(LIB_PATH)/ArduinoInterruptExpander.a $(CPP_OBJECTS)
	ar -rvs $(LIB_PATH)/ArduinoInterruptExpander.a $(C_OBJECTS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c* | $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC_PATH)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC_PATH)

clean: 
	rm -rf $(LIB_PATH)
	rm -rf $(OBJ_PATH)
