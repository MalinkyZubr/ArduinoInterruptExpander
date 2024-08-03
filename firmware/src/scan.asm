.include "../include/tn85def.inc"
.include "../include/constants.inc"
.include "./gpio_setup.asm"
.include "./spi.asm"
.include "./gpio_ops.asm"

.def clockMask = r16 ; holds toggle 1 for xor the plex clock value
.def readMask = r17 ; bitmask for read port
.def readValue = r19 ; the value read from the read pin

.def temp1 = r20 ; misc general register
.def temp2 = r21 ; misc general register


handlePlexInput:
    push temp1
    readPortMacro temp1, plexReadPortMask
    cpi temp1, 0
    breq skipSPI; if there is no input from multiplexer skip the spi

    push temp2
    mov temp2, scanCounter
    SPITransferMacro temp2
    pop temp2
skipSPI: 
    pop temp1
    ret

scanMainLoop:
start_loop:
    ldi scanCounter, 0
next_iteration:
    pulsePinMacro temp2, plexClockPortMask

    rcall handlePlexInput

    inc scanCounter
    cpi scanCounter, maxInputAddress
    brne next_iteration

    rjmp start_loop

    ret

