.include "../include/tn85def.inc"
.include "../include/constants.inc"
.include "./gpio_setup.asm"


.def configBitmask = r16
.def dataRegister = r17

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
    readPinMacro dataRegister, SPIChipSelectMask
    andi dataRegister, 1
    brne awaitSelect
    pop dataRegister

    ret

slaveSPITransfer:
    out USIDR, dataRegister
    ldi configBitmask, (1<<USIOIF)
    out USISR, configBitmask
slaveSPITransferLoop:
    in dataRegister, USISR
    sbrs dataRegister, USIOIF
    rjmp SlaveSPITransferLoop
    in dataRegister, USIDR

    ret

; @0: register holding data to be transmitted to master
.macro SPITransferMacro @0
    push dataRegister
    push configBitmask

    mov dataRegister, @0
    rcall SPIInterruptMaster
    rcall SPIAwaitSelect
    rcall slaveSPITransfer

    pop configBitmask
    pop dataRegister
.endmacro
