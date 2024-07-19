#ifndef VIRTUALINTERRUPTMANAGER_H
#define VIRTUALINTERRUPTMANAGER_H

#include "VirtualInterrupt.h"


enum TriggerInteruptPin {
    TRIG_INT_1 = 2,
    TRIG_INT_2 = 3
};


enum VIManagerReturn {
    VI_OP_SUCCESS,
    VI_ADDRESS_NOT_LOADED,
    VI_ADDRESS_OCCUPIED,
    VI_IMMUTABLE,
    VI_DISABLED
};


class VirtualInterruptManager {
    private:
    int clock_state = 0;
    int currently_reading = 0;
    TriggerInteruptPin interrupt_pin;

    VirtualInterruptFrame current_buffer;
    VirtualInterrupt interrupt_table[32];

    void enable_input_trigger();
    void disable_input_trigger();

    public:
    VirtualInterruptManager(TriggerInteruptPin interrupt_pin);
    VIManagerReturn attachVIInterrupt(InterruptAddress interrupt_address, VirtualISR isr, int immutable);
    VIManagerReturn modifyVIInterrupt(InterruptAddress interrupt_address, VirtualISR isr);
    VIManagerReturn detachVIInterrupt(InterruptAddress interrupt_address);
    
    void enableVIInterrupt(InterruptAddress interrupt_address);
    void disableVIInterrupt(InterruptAddress interrupt_address);
    void triggerVIInterrupt(InterruptAddress interrupt_address);
};


#endif