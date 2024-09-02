#ifndef VIRTUALINTERRUPTMANAGER_HPP
#define VIRTUALINTERRUPTMANAGER_HPP

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

#include <stdio.h>


enum VIManagerReturn {
    VI_OP_SUCCESS,
    VI_ADDRESS_NOT_LOADED,
    VI_ADDRESS_OCCUPIED,
    VI_IMMUTABLE,
    VI_DISABLED,
    VI_TASK_QUEUE_EMPTY
};


class VirtualInterruptManager {
    private:
    VirtualInterrupt interrupt_table[64];
    VITaskQueue *task_queue = NULL;

    public:
    VirtualInterruptManager() {}
    VirtualInterruptManager(VITaskQueue *task_queue);

    VIManagerReturn attachVIInterrupt(InterruptAddress interrupt_address, VirtualISR isr, int immutable);
    VIManagerReturn modifyVIInterrupt(InterruptAddress interrupt_address, VirtualISR isr);
    VIManagerReturn detachVIInterrupt(InterruptAddress interrupt_address);
    
    VIManagerReturn enableVIInterrupt(InterruptAddress interrupt_address);
    VIManagerReturn disableVIInterrupt(InterruptAddress interrupt_address);
    VIManagerReturn triggerVIInterrupt(InterruptAddress interrupt_address);

    VIManagerReturn runTaskFromQueue();
};

void SPISetup(uint8_t vector);

#endif