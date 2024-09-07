.include "../include/tn85def.inc"

.def configBitmask = r16
.def dataRegister = r17
.def temp1 = r20 ; misc general register
.def temp2 = r21 ; misc general register
.def ioTempReg = r23
.def bitmaskReg = r24
.def scanCounter = r18 ; maximum 64

.equ maxInputAddress = 65 ; maximum read address for interrupt expander
.equ plexClockMask = (1<<PORTB0)
.equ SPIDataOutMask = (1<<PORTB1)
.equ SPIClockMask = (1<<PORTB2)
.equ plexReadMask = (1<<PORTB3) ;handles clock and reading
.equ SPIChipSelectMask = (1<<PORTB4) ; doubles as interrupt port to tell master to initiate communications


.macro setupStack
    push r16

    ldi r16,LOW(@0)
    out SPL, r16
    ldi r16,HIGH(@0)
    out SPH,r16

    pop r16
.endmacro

.macro acquireGPIORegisters
    push ioTempReg
    push bitmaskReg
.endmacro

.macro releaseGPIORegisters
    pop bitmaskReg
    pop ioTempReg
.endmacro

; @0: constant containing the desired bitmask for the port bit
.macro togglePortMacro
    acquireGPIORegisters

    ldi bitmaskReg, @0

    rcall togglePort

    releaseGPIORegisters
.endmacro

; @0: constant containing the desired bitmask for the port bit
.macro pulsePortMacro
    acquireGPIORegisters
    ldi bitmaskReg, @0

    rcall togglePort
    rcall delay
    rcall togglePort

    releaseGPIORegisters
.endmacro

; @0: register that will store sensor output
; @1: constant containing the desired bitmask for the port bit
.macro readPortMacro
    acquireGPIORegisters

    ldi bitmaskReg, @1
    rcall readPort
    mov @0, ioTempReg

    releaseGPIORegisters
.endmacro

; @0: constant containing the desired bitmask for the port bit
.macro setPortInputMacro
    acquireGPIORegisters

    ldi bitmaskReg, @0
    rcall setPortInput

    releaseGPIORegisters
.endmacro

; @0 constant containing desired bitmaks for the port bit
.macro setPortOutputMacro
    acquireGPIORegisters

    ldi bitmaskReg, @0
    rcall setPortOutput

    releaseGPIORegisters
.endmacro

; @0 constant containing desired bitmask for port bit
.macro setPortOutputLowMacro
    acquireGPIORegisters

    ldi bitmaskReg, @0
    rcall setPortOutputLow

    releaseGPIORegisters
.endmacro

; @0 constant containing desired bitmask for port bit
.macro setPortOutputHighMacro
    acquireGPIORegisters

    ldi bitmaskReg, @0
    rcall setPortOutputHigh

    releaseGPIORegisters
.endmacro

; @0 label for a subroutine
.macro runSubroutineInLoop
    subroutine_loop:
    rcall @0
    rjmp subroutine_loop
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

.set TEST_NUM = 6
.cseg
.org 0x00

;rcall spazz
rcall start

tests:
.if TEST_NUM == 1
    rcall spazz
.endif


.if TEST_NUM == 2
    rcall testSPISend
.endif


.if TEST_NUM == 3
    rcall testInterruptTrigger
.endif


.if TEST_NUM == 4
    rcall testGPIOOpts
.endif


.if TEST_NUM == 5
    setPortOutputMacro plexClockMask
    setPortOutputLowMacro plexClockMask
    rcall testSPIAwaitSelect
    setPortOutputHighMacro plexClockMask
.endif

.if TEST_NUM == 6
    runSubroutineInLoop spazz
.endif

.if TEST_NUM == 0
main:
    rcall scanMainLoop

    rjmp main
.endif

spazz:
    spazz_start:
    togglePortMacro SPIChipSelectMask

    rcall delay

    togglePortMacro SPIChipSelectMask

    rcall delay

    rjmp spazz_start

    ret

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


testOutputPins: ; simple test to set pin to output so oscilloscope can read state
    setPortOutputMacro SPIChipSelectMask

    rcall delay
    rcall delay
    rcall delay
    togglePortMacro SPIChipSelectMask

    rcall delay

    togglePortMacro SPIChipSelectMask

    rcall delay

    setPortOutputLowMacro SPIChipSelectMask
    ;togglePortMacro SPIChipSelectMask

    ret

testSPISend: ;; send a sample message out the SPI interface for oscilloscope to read
    push r16
    ldi r16, 0b10111010
    SPITransferMacro r16

    pop r16
    ret

testSPISendReal:
    push r16
    ldi r16, 0b00010001
    SPITransferMacro r16

    pop r16
    ret

testInterruptTrigger: ; didnt get a signal
    rcall SPIInterruptMaster
    ret

testGPIOOpts:
    push r16

    setPortOutputMacro SPIChipSelectMask
    setPortOutputHighMacro SPIChipSelectMask

    setPortInputMacro plexReadMask

    gpio_test_loop: ; await a high signal to the input gpio
    readPortMacro r16, plexReadMask
    tst r16
    breq gpio_test_loop

    setPortOutputLowMacro SPIChipSelectMask

    pop r16

    ret

testSPIAwaitSelect:
    setPortOutputMacro plexReadMask
    setPortOutputHighMacro plexReadMask
    rcall SPIAwaitSelect
    setPortOutputLowMacro plexReadMask

    ret

start:
    setupStack RAMEND
    rcall GPIOSetup
    rcall SPISetup

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
    ;setPortOutputMacro SPIChipSelectMask
    pulsePortMacro SPIChipSelectMask
    ;setPortInputMacro SPIChipSelectMask

    ret

SPIAwaitSelect: ; add code here that will handle a timeout
    push dataRegister
    setPortInputMacro SPIChipSelectMask
    setPortOutputHighMacro SPIChipSelectMask
awaitSelect:
    readPortMacro dataRegister, SPIChipSelectMask
    ;rcall delay ; delete this later maybe
    tst dataRegister
    brne awaitSelect ; selected at logical low

    setPortOutputMacro SPIChipSelectMask
    setPortOutputLowMacro SPIChipSelectMask

    rcall delay

    pop dataRegister

    ret

slaveSPITransfer:
    out USIDR, dataRegister
    ldi configBitmask, (1<<USIOIF)
    out USISR, configBitmask
    setPortOutputMacro SPIDataOutMask
slaveSPITransferLoop:
    sbis USISR, USIOIF
    rjmp slaveSPITransferLoop
    
    in dataRegister, USISR
    ori dataRegister, (1<<USIOIF)
    out USISR, dataRegister

    setPortInputMacro SPIDataOutMask

    ret

GPIOSetup:
    setPortOutputMacro SPIDataOutMask
    setPortOutputMacro plexClockMask
    setPortOutputMacro SPIChipSelectMask

    setPortInputMacro SPIClockMask
    setPortInputMacro plexReadMask

    ret


handlePlexInput:
    push temp1
    push ioTempReg
    
    readPortMacro temp1, plexReadMask
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
    pulsePortMacro plexClockMask
    rcall handlePlexInput

    inc scanCounter
    cpi scanCounter, maxInputAddress
    brne next_iteration

    rjmp start_loop

    ret

silly:
    nop