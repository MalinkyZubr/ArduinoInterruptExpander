#ifndef SPI_H
#define SPI_H

#include <cstdint>

#define MSBFIRST 0
#define SPI_MODE0 0

class SPISettings {
    public:
    SPISettings();
    SPISettings(int rate, int first, int mode);
};


class SPIClass {
    public:
    void beginTransaction(const SPISettings settings);
    void endTransaction();
    void usingInterrupt(int vector);

    uint8_t transfer(uint8_t send_byte);
};

extern SPIClass SPI;

#endif