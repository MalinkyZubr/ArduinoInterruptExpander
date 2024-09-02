#include "./include/SPI.h"


SPIClass SPI;

SPISettings::SPISettings() {}
SPISettings::SPISettings(int rate, int first, int mode) {}

void SPIClass::beginTransaction(const SPISettings settings) {}
void SPIClass::endTransaction() {}
void SPIClass::usingInterrupt(int vector) {}

uint8_t SPIClass::transfer(uint8_t send_byte) {
    return 0b00000000;
}