#ifndef SPI_H
#define SPI_H

#include <cstdint>


typedef struct SPISettings {} SPISettings;


class SPIClass {
    public:
    void beginTransaction(const SPISettings settings);
    void endTransaction();
    void usingInterrupt(int vector);

    uint8_t transfer(uint8_t send_byte);
};

extern SPIClass SPI;

#endif