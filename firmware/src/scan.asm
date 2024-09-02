.include "../include/tn85def.inc"
.include "../include/constants.inc"
.include "./gpio_setup.asm"
.include "./spi.asm"
.include "./gpio_ops.asm"


.def temp1 = r20 ; misc general register
.def temp2 = r21 ; misc general register


handlePlexInput:
    push temp1
    push temp2
    
    readPortMacro temp1, plexPortMask
    cpi temp1, 0
    breq skipSPI; if there is no input from multiplexer skip the spi

    mov temp2, scanCounter
    SPITransferMacro temp2
skipSPI: 
    pop temp2
    pop temp1
    ret

scanMainLoop:
start_loop:
    ldi scanCounter, 0
next_iteration:
    setPortOutputMacro plexPortMask
    pulsePinMacro temp2, plexPortMask
    setPortInputMacro plexPortMask

    rcall handlePlexInput

    inc scanCounter
    cpi scanCounter, maxInputAddress
    brne next_iteration

    rjmp start_loop

    ret

