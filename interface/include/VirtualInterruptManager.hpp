#ifndef VIRTUALINTERRUPTMANAGER_H
#define VIRTUALINTERRUPTMANAGER_H

#include <Arduino.h>
#include "VirtualInterrupt.h"
#include "VirtualInterruptSetup.h"
#include "VirtualInterruptTaskQueue.hpp"
#include "./include/AtomicArduino/include/atomic.hpp"


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
const VISPISettings = SPISettings(4000000, MSBFIRST, SPI_MODE0)

ISR(VI_INTERRUPT_VECTOR) {
    ATOMIC_OPERATION(() -> void {
            pinMode(VI_CS_PIN, OUTPUT);
            digitalWrite(VI_CS_PIN, LOW);

            uint8_t interrupt_address = VIRead(0b11111111); // put SPISettings object here)
            VI_Manager.triggerVIINterrupt(interrupt_address);

            pinMode(VI_CS_PIN, INPUT_PULLUP);
        }
    )
}


#endif