.include "../include/tn85def.inc"
.include "../include/constants.inc"


.def ioTempReg = r23
.def bitmaskReg = r24


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
    and ioTempReg, ~bitmaskReg
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
    and ioTempReg, ~bitmaskReg
    out DDRB, ioTempReg

    ret

setPortOutput:
    in ioTempReg, DDRB
    or ioTempReg, bitmaskReg
    out DDRB, ioTempReg

    ret

; @0: constant containing the desired bitmask for the port bit
.macro togglePortMacro @0
    rcall acquireGPIORegisters

    ldi bitmaskReg, @0

    rcall togglePort

    rcall releaseGPIORegisters
.endmacro

; @0: constant containing the desired bitmask for the port bit
.macro pulsePortMacro @0
    rcall acquireGPIORegisters
    ldi bitmaskReg, @0

    rcall togglePort
    nop
    rcall togglePort

    rcall releaseGPIORegisters
.endmacro

; @0: register that will store sensor output
; @1: constant containing the desired bitmask for the port bit
.macro readPortMacro @0, @1
    rcall acquireGPIORegisters

    ldi bitmaskReg, @1
    rcall readPort
    mov @0, ioTempReg

    rcall releaseGPIORegisters
.endmacro

; @0: constant containing the desired bitmask for the port bit
.macro setPortInputMacro @0
    rcall acquireGPIORegisters

    ldi bitmaskReg, @0
    rcall setPortInput

    rcall releaseGPIORegisters
.endmacro

; @0 constant containing desired bitmaks for the port bit
.macro setPortOutputMacro @0
    rcall acquireGPIORegisters

    ldi bitmaskReg, @0
    rcall setPortOutput

    rcall releaseGPIORegisters
.endmacro