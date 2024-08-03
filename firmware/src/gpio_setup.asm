.include "../include/tn85def.inc"
.include "../include/constants.inc"


GPIOSetup:
    push r16

    ldi r16, SPIDataOutMask | SPIClockMask | plexClockPortMask
    out DDRB, r16

    pop r16

    ret



