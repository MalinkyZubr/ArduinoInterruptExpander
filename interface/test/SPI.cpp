#include "./include/SPI.h"


SPIClass SPI;

void SPIClass::beginTransaction(const SPISettings settings) {}
void SPIClass::endTransaction() {}
void SPIClass::usingInterrupt(int vector) {}

uint8_t SPIClass::transfer(uint8_t send_byte) {
    return 0b00000000;
}