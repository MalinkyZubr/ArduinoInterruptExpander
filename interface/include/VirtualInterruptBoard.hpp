#ifndef ACTIVATE_HPP
#define ACTIVATE_HPP


#include <SPI.h>
#include <Arduino.h>
#include "./VirtualInterruptManager.hpp"


VITaskQueue task_queue = VITaskQueue();
VirtualInterruptManager VI_Manager = VirtualInterruptManager(task_queue);

ISR(VI_INTERRUPT_VECTOR) {
    cli();

    pinMode(VI_CS_PIN, OUTPUT);
    digitalWrite(VI_CS_PIN, LOW);

    //const uint8_t send_address = 0b11111111;
    uint8_t interrupt_address = VIRead(0b11111111, SPISettings(4000000, MSBFIRST, SPI_MODE0)); // put SPISettings object here)
    VI_Manager.triggerVIInterrupt((InterruptAddress)interrupt_address);

    pinMode(VI_CS_PIN, INPUT_PULLUP);

    sei();
}

#endif