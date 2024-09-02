#ifndef ACTIVATE_HPP
#define ACTIVATE_HPP


#include <SPI.h>
#include <Arduino.h>
#include "./VirtualInterruptManager.hpp"

#define TEST_BUILD 0


extern VITaskQueue task_queue;
extern VirtualInterruptManager VI_Manager;

inline void VI_Routine() {
    cli();

    pinMode(VI_CS_PIN, OUTPUT);
    digitalWrite(VI_CS_PIN, LOW);

    //const uint8_t send_address = 0b11111111;
    uint8_t interrupt_address = VIRead(0b11111111, SPISettings(4000000, MSBFIRST, SPI_MODE0)); // put SPISettings object here)
    VI_Manager.triggerVIInterrupt((InterruptAddress)interrupt_address);

    pinMode(VI_CS_PIN, INPUT_PULLUP);

    sei();
}

#if TEST_BUILD == 1
ISR(VI_INTERRUPT_VECTOR) {
    VI_Routine();
}
#endif

#endif