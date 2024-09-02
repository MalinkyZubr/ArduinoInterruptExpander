TEST_N?=0
INCLUDE_PATH=./include
SRC_PATH=./src 
AVR_FLAGS=--max_errors 10000 --define TEST_NUM=$(TEST_N)
AVRDUDE_FLAGS=-c stk500v1 -p attiny85 -P /dev/ttyACM0 -b 19200
OBJ_PATH=./obj
AVR=avra

ASM_SOURCE_PATH:=./src
OBJ_PATH:=./obj

all: compile upload

compile: $(OBJ_PATH)
	$(AVR) $(ASM_SOURCE_PATH)/main.asm -I $(INCLUDE_PATH)
	mv $(ASM_SOURCE_PATH)/main.cof $(OBJ_PATH)/main.cof
	mv $(ASM_SOURCE_PATH)/main.eep.hex $(OBJ_PATH)/main.eep.hex
	mv $(ASM_SOURCE_PATH)/main.hex $(OBJ_PATH)/main.hex
	mv $(ASM_SOURCE_PATH)/main.obj $(OBJ_PATH)/main.obj

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)	

upload:
	avrdude $(AVRDUDE_FLAGS) -U flash:w:$(OBJ_PATH)/main.hex

clean:
	rm -rf $(OBJ_PATH)