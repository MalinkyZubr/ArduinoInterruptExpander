#include "../include/VirtualInterrupt.hpp"


VirtualInterrupt instantiate_interrupt(enum InterruptAddress interrupt_address) {
    VirtualInterrupt interrupt;
    interrupt.initialized = 0;
    interrupt.enabled = 0;
    interrupt.immutable = 0;
    interrupt.address = interrupt_address;
    return interrupt;
}

uint8_t VIRead(const uint8_t send_byte, const SPISettings spi_settings) {
    SPI.beginTransaction(spi_settings);
    uint8_t received_byte = SPI.transfer(send_byte);
    SPI.endTransaction();

    return received_byte;
}