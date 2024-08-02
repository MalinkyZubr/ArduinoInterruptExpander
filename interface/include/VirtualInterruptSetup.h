#ifndef VIRTUALINTERRUPTSETUP_H
#define VIRTUALINTERRUPTSETUP_H

#include <Arduino.h>
#include "VirtualInterrupt.h"


// assuming 16 mhz clock
enum PrescalerDivisor {
    PRESCALE_NONE = 0b00000001,
    PRESCALE_8 = 0b00000010,
    PRESCALE_64 = 0b00000011,
    PRESCALE_256 = 0b00000100,
    PRESCALE_1024 = 0b00000101
};


int set_prescaler(enum PrescalerDivisor prescaler_type);
int set_timers(int reset_value);
int set_interrupts();

#endif