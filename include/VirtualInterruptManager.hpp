#ifndef VIRTUALINTERRUPTMANAGER_H
#define VIRTUALINTERRUPTMANAGER_H

#include "VirtualInterrupt.h"


enum VIReadPin {
    VI_INT_1 = 2,
    VI_INT_2 = 3
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
    VIReadPin read_pin;
    int clock_pin;

    VirtualInterruptFrame current_buffer;
    VirtualInterrupt interrupt_table[32];

    void enable_input_trigger();
    void disable_input_trigger();
    void trigger_reading_state();

    public:
    VirtualInterruptManager(VIReadPin read_pin, int clock_pin);
    VIManagerReturn attachVIInterrupt(InterruptAddress interrupt_address, VirtualISR isr, int immutable);
    VIManagerReturn modifyVIInterrupt(InterruptAddress interrupt_address, VirtualISR isr);
    VIManagerReturn detachVIInterrupt(InterruptAddress interrupt_address);
    
    void enableVIInterrupt(InterruptAddress interrupt_address);
    void disableVIInterrupt(InterruptAddress interrupt_address);
    void triggerVIInterrupt(InterruptAddress interrupt_address);

    void stop_reading();
    int is_reading();
};

VirtualInterruptManager VI_Manager = VirtualInterruptManager(VI_INT_1, 4);

ISR(TIMER1_COMPA_vect) {
    VITimerInterruptPWM(x, y);

    if(VI_Manager.is_reading()) {
        // do other things
    }
}


#endif