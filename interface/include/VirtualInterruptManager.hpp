#ifndef VIRTUALINTERRUPTMANAGER_H
#define VIRTUALINTERRUPTMANAGER_H

#include "VirtualInterrupt.h"
#include "VirtualInterruptSetup.h"
#include "../include/AtomicArduino/include/atomic.hpp"


enum VICSPin {
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
    VirtualInterrupt interrupt_table[64];
    VICSPin cs_pin

    void enable_input_trigger();
    void disable_input_trigger();

    public:
    VirtualInterruptManager(VICSPin cs_pin);
    VIManagerReturn attachVIInterrupt(InterruptAddress interrupt_address, VirtualISR isr, int immutable);
    VIManagerReturn modifyVIInterrupt(InterruptAddress interrupt_address, VirtualISR isr);
    VIManagerReturn detachVIInterrupt(InterruptAddress interrupt_address);
    
    void enableVIInterrupt(InterruptAddress interrupt_address);
    void disableVIInterrupt(InterruptAddress interrupt_address);
    void triggerVIInterrupt(InterruptAddress interrupt_address);
};


VirtualInterruptManager VI_Manager = VirtualInterruptManager(VI_INT_1, 4);

ISR(TIMER1_COMPA_vect) {
    ATOMIC_OPERATION(() -> void {
            VIClockManager* clock_manager = VI_Manager.get_clock_manager();
            clock_manager->clock_state = VITimerInterruptPWM(clock_manager->clock_pin, clock_manager->clock_state);

            if(global_reading_flag == 1) {
                VirtualInterruptFrame* VIBuffer = VI_Manager.getVIBuffer();
                VITimerInterruptRead(VI_Manager.get_read_pin(), VIbuffer);

                if(VIBuffer->bits_received == 8) {
                    if(VITimerInterruptErrorCheck(VIBuffer->received_address)) {
                        // error correction procedure here
                    }
                    else {
                        VI_Manager.triggerVIInterrupt(VIBuffer->received_address);

                        if(VITimerCheckContinuationBit(VIBuffer->received_address)) {
                            // continuiation procedure here
                        }
                    }
                }
            }
        }
    )
}


#endif