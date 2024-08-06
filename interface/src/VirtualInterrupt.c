#include "../include/VirtualInterrupt.h"


VirtualInterrupt instantiate_interrupt(enum InterruptAddress interrupt_address) {
    VirtualInterrupt interrupt;
    interrupt.address = interrupt_address;
    return interrupt;
}

uint8_t VITimerInterruptRead(uint8_t send_byte, SPISettings* spi_settings) {
    SPI.beginTransaction(spi_settings);
    uint8_t received_byte = SPI.transfer(send_byte);
    SPI.endTransaction();

    return received_byte;
}