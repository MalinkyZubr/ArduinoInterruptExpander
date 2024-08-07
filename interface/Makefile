# -*- MakeFile -*-

INC_PATH=-I./include -I/usr/share/arduino/hardware/arduino/avr/cores/arduino/ -I/usr/share/arduino/hardware/arduino/avr/libraries
LIB_PATH=-L./lib
CC=g++
CFLAGS=-mmcu atmega328p -O2
SOURCES=$(wildcard ./src/*.c) $(wildcard ./src/*.cpp)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
OBJECTS_CPP=$(patsubst %.c, %.o, $(SOURCES))


all: build $(LIB_PATH)

$(LIB_PATH): $(OBJECTS)
	ar rvs ArduinoInterruptExpander.a $(OBJECTS)

$(OBJECTS): ./src/obj/%.o : ./src/%.c ./src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INC_PATH)

