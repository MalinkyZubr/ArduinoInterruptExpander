.include "../include/tn85def.inc"


.equ SPIChipSelectMask = (1<<PORTB4) ; doubles as interrupt port to tell master to initiate communications
.def ioTempReg = r23
.def bitmaskReg = r24


.macro setupStack
    ldi r16,LOW(@0)
    out SPL, r16
    ldi r16,HIGH(@0)
    out SPH,r16
.endmacro

.macro togglePortMacro
    acquireGPIORegisters

    ldi bitmaskReg, @0

    rcall togglePort

    releaseGPIORegisters
.endmacro

.macro acquireGPIORegisters
    push ioTempReg
    push bitmaskReg
.endmacro

.macro releaseGPIORegisters
    pop bitmaskReg
    pop ioTempReg
.endmacro


.macro setPortOutputMacro
    acquireGPIORegisters

    ldi bitmaskReg, @0
    rcall setPortOutput

    releaseGPIORegisters
.endmacro

.macro setPortOutputHighMacro
    acquireGPIORegisters

    ldi bitmaskReg, @0
    rcall setPortOutputHigh

    releaseGPIORegisters
.endmacro


.macro setPortOutputLowMacro
    acquireGPIORegisters

    ldi bitmaskReg, @0
    rcall setPortOutputLow

    releaseGPIORegisters
.endmacro



.cseg
.org 0x00

setupStack RAMEND
start:
    ; setPortOutputMacro SPIChipSelectMask
    ; setPortOutputHighMacro SPIChipSelectMask
    ;ldi bitmaskReg, SPIChipSelectMask
    ;rcall acquireGPIORegisters
    ;rcall releaseGPIORegisters
    ; rcall delay
    ; setPortOutputHighMacro SPIChipSelectMask

    rcall delay
    setPortOutputLowMacro SPIChipSelectMask

    rjmp start

delay: ; very basic delay routine with no inputs to use for tests
    push r16

    ldi r16, 255
    delay_loop:
    nop
    nop
    subi r16, 1
    brne delay_loop

    pop r16

    ret


togglePort:
    in ioTempReg, PORTB
    eor ioTempReg, bitmaskReg
    out PORTB, ioTempReg

    ret


setPortOutput:
    in ioTempReg, DDRB
    or ioTempReg, bitmaskReg
    out DDRB, ioTempReg

    ret


setPortOutputHigh:
    in ioTempReg, PORTB
    or ioTempReg, bitmaskReg
    out PORTB, ioTempReg
    
    ret


setPortOutputLow:
    in ioTempReg, PORTB
    com bitmaskReg
    and ioTempReg, bitmaskReg
    
    out PORTB, ioTempReg

    ret