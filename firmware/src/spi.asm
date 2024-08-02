.include "../include/tn85def.inc"
.include "../include/constants.inc"


.def configBitmask = r16
.def dataRegister

start:
    push configBitmask

    ldi configBitmask, (1<<USIWM0) | (1<<USICS1)
    out USICR, configBitmask

