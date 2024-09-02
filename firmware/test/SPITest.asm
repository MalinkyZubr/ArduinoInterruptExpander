.include "../include/tn85def.inc"
.include "../include/constants.inc"
.include "../src/gpio_setup.asm"
.include "../src/spi.asm"


start:
    rcall SPISetup
    