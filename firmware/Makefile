INCLUDE_PATH=./include
SRC_PATH=./src 
AVR_FLAGS=--max_errors 10000
AVRDUDE_FLAGS=-c stk500v1 -p attiny85 -P /dev/ttyACM0 -b 19200
OBJ_PATH=./obj
AVR=avra

ASM_SOURCE_PATH:=./src
OBJ_PATH:=./obj


main:
	avra $(ASM_SOURCE_PATH)/main.asm -I $(INCLUDE_PATH) -o ./obj
	avrdude $(AVRDUDE_FLAGS) -U flash:w:$(ASM_SOURCE_PATH)/main.hex
