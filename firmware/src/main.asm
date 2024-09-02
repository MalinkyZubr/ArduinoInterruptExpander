.include "../include/tn85def.inc"
.include "../include/constants.inc"
.include "./gpio_setup.asm"
.include "./gpio_ops.asm"
.include "./scan.asm"
.include "./spi.asm"


start:
    setupStack RAMEND
    rcall GPIOSetup
    rcall SPISetup

main:
    rcall scanMainLoop