.include "../include/tn85def.inc"


.macro setupStack
    push r16

    ldi r16,LOW(@0)
    out SPL, r16
    ldi r16,HIGH(@0)
    out SPH,r16

    pop r16
.endmacro

; @0: constant containing the desired bitmask for the port bit
.macro togglePortMacro
    rcall acquireGPIORegisters

    ldi bitmaskReg, @0

    rcall togglePort

    rcall releaseGPIORegisters
.endmacro

; @0: constant containing the desired bitmask for the port bit
.macro pulsePortMacro
    rcall acquireGPIORegisters
    ldi bitmaskReg, @0

    rcall togglePort
    nop
    rcall togglePort

    rcall releaseGPIORegisters
.endmacro

; @0: register that will store sensor output
; @1: constant containing the desired bitmask for the port bit
.macro readPortMacro
    rcall acquireGPIORegisters

    ldi bitmaskReg, @1
    rcall readPort
    mov @0, ioTempReg

    rcall releaseGPIORegisters
.endmacro

; @0: constant containing the desired bitmask for the port bit
.macro setPortInputMacro
    rcall acquireGPIORegisters

    ldi bitmaskReg, @0
    rcall setPortInput

    rcall releaseGPIORegisters
.endmacro

; @0 constant containing desired bitmaks for the port bit
.macro setPortOutputMacro
    rcall acquireGPIORegisters

    ldi bitmaskReg, @0
    rcall setPortOutput

    rcall releaseGPIORegisters
.endmacro

; @0: register holding data to be transmitted to master
.macro SPITransferMacro
    push dataRegister
    push configBitmask
    push r18
    push r19

    mov dataRegister, @0
    rcall SPIInterruptMaster
    rcall SPIAwaitSelect
    rcall slaveSPITransfer

    pop r19
    pop r18
    pop configBitmask
    pop dataRegister
.endmacro

.def configBitmask = r16
.def dataRegister = r17
.def temp1 = r20 ; misc general register
.def temp2 = r21 ; misc general register
.def ioTempReg = r23
.def bitmaskReg = r24
.def scanCounter = r18 ; maximum 64

.equ maxInputAddress = 65 ; maximum read address for interrupt expander
.equ SPIDataInMask = (1<<PORTB0)
.equ SPIDataOutMask = (1<<PORTB1)
.equ SPIClockMask = (1<<PORTB2)
.equ plexPortMask = (1<<PORTB3) ;handles clock and reading
.equ SPIChipSelectMask = (1<<PORTB4) ; doubles as interrupt port to tell master to initiate communications

.cseg
.org 0x00

main:
    rcall start
    rcall scanMainLoop

start:
    setupStack RAMEND
    rcall GPIOSetup
    rcall SPISetup

    ret

acquireGPIORegisters:
    push ioTempReg
    push bitmaskReg

    ret

releaseGPIORegisters:
    pop bitmaskReg
    pop ioTempReg

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

togglePort:
    in ioTempReg, PORTB
    eor ioTempReg, bitmaskReg
    out PORTB, ioTempReg

    ret

readPort:
    in ioTempReg, PINB
    and ioTempReg, bitmaskReg
    breq isLow

    ldi ioTempReg, 1
    rjmp endif
isLow:
    ldi ioTempReg, 0
endif:
    ret

setPortInput:
    in ioTempReg, DDRB
    com bitmaskReg
    and ioTempReg, bitmaskReg
    out DDRB, ioTempReg

    ret

setPortOutput:
    in ioTempReg, DDRB
    or ioTempReg, bitmaskReg
    out DDRB, ioTempReg

    ret

; remember to set DO and USCK pins as outputs

SPISetup:
    push configBitmask

    ldi configBitmask, (1<<USIWM0) | (1<<USICS1)
    out USICR, configBitmask

    pop configBitmask

    ret

SPIInterruptMaster:
    setPortOutputMacro SPIChipSelectMask ; optimize to just pass a register later
    pulsePortMacro SPIChipSelectMask
    setPortInputMacro SPIChipSelectMask

    ret

SPIAwaitSelect: ; add code here that will handle a timeout
    push dataRegister
awaitSelect:
    readPortMacro dataRegister, SPIChipSelectMask
    andi dataRegister, 1
    brne awaitSelect

    pop dataRegister

    ret

slaveSPITransfer:
    out USIDR, dataRegister
    ldi configBitmask, (1<<USIOIF)
    out USISR, configBitmask
    setPortInputMacro SPIDataInMask
    setPortOutputMacro SPIDataOutMask
slaveSPITransferLoop:
    in dataRegister, USISR
    sbrs dataRegister, USIOIF
    rjmp SlaveSPITransferLoop
    in dataRegister, USIDR

    setPortOutputMacro SPIDataInMask
    setPortInputMacro SPIDataOutMask

    ret

GPIOSetup:
    setPortOutputMacro SPIDataOutMask
    setPortOutputMacro SPIDataOutMask

    setPortInputMacro SPIChipSelectMask
    setPortInputMacro plexPortMask

    ret


handlePlexInput:
    push temp1
    push ioTempReg
    
    readPortMacro temp1, plexPortMask
    cpi temp1, 0
    breq skipSPI; if there is no input from multiplexer skip the spi

    mov ioTempReg, scanCounter
    SPITransferMacro ioTempReg
skipSPI: 
    pop ioTempReg
    pop temp1
    ret

scanMainLoop:
start_loop:
    ldi scanCounter, 0
next_iteration:
    setPortOutputMacro plexPortMask
    pulsePortMacro plexPortMask
    setPortInputMacro plexPortMask

    rcall handlePlexInput

    inc scanCounter
    cpi scanCounter, maxInputAddress
    brne next_iteration

    rjmp start_loop

    ret

;.include "./subroutines.asm"