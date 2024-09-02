.include "../include/tn85def.inc"
.include "../include/constants.inc"
.include "./gpio_ops.asm"


GPIOSetup:
    setPortOutputMacro SPIDataOutMask
    setPortOutputMacro SPIDataOutMask

    setPortInputMacro SPIChipSelectMask
    setPortInputMacro plexPortMask

    ret



