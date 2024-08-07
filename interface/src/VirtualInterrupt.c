#include "../include/VirtualInterrupt.h"


VirtualInterrupt instantiate_interrupt(enum InterruptAddress interrupt_address) {
    VirtualInterrupt interrupt;
    interrupt.address = interrupt_address;
    return interrupt;
}

uint8_t VIRead(const uint8_t send_byte, const SPISettings* spi_settings) {
    SPI.beginTransaction(spi_settings);
    uint8_t received_byte = SPI.transfer(send_byte);
    SPI.endTransaction();

    return received_byte;
}