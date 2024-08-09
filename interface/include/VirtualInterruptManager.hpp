#ifndef VIRTUALINTERRUPTMANAGER_H
#define VIRTUALINTERRUPTMANAGER_H

#include <Arduino.h>
#include <SPI.h>
#include "VirtualInterrupt.hpp"
#include "VirtualInterruptTaskQueue.hpp"
#include <io.h>


#ifndef VI_CS_PIN
#define VI_CS_PIN 2
#endif

#if VI_CS_PIN == 2
#define VI_INTERRUPT_VECTOR INT0_vect
#elif VI_CS_PIN == 3
#define VI_INTERRUPT_VECTOR INT1_vect
#else
#error "Use UNO interrupt pins for VI_CS_PIN"
#endif


enum VIManagerReturn {
    VI_OP_SUCCESS,
    VI_ADDRESS_NOT_LOADED,
    VI_ADDRESS_OCCUPIED,
    VI_IMMUTABLE,
    VI_DISABLED
};


class VirtualInterruptManager {
    private:
    VirtualInterrupt interrupt_table[64];
    VITaskQueue &task_queue;

    public:
    VirtualInterruptManager(VITaskQueue &task_queue);

    VIManagerReturn attachVIInterrupt(InterruptAddress interrupt_address, VirtualISR isr, int immutable);
    VIManagerReturn modifyVIInterrupt(InterruptAddress interrupt_address, VirtualISR isr);
    VIManagerReturn detachVIInterrupt(InterruptAddress interrupt_address);
    
    void enableVIInterrupt(InterruptAddress interrupt_address);
    void disableVIInterrupt(InterruptAddress interrupt_address);
    void triggerVIInterrupt(InterruptAddress interrupt_address);
};


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