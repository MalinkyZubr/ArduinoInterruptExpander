#include "../include/VirtualInterrupt.h"


VirtualInterrupt instantiate_interrupt(enum InterruptAddress interrupt_address) {
    VirtualInterrupt interrupt;
    interrupt.address = interrupt_address;
    return interrupt;
}

char VIExternalIntReceiveTriggerBit() {
    return 0b00000001;
}

int VITimerInterruptPWM(int clock_pin, int clock_state) {
    clock_state ^= 1;
    digitalWrite(clock_pin, clock_state); // refactor to direct access
    return clock_state;
}

void VITimerInterruptRead(int read_pin, VirtualInterruptFrame* frame_buffer) {
    char read_value = (char) digitalRead(read_pin);
    read_value <<= frame_buffer->bits_received;
    frame_buffer->interrupt_address |= read_value;

    frame_buffer->bits_received++;
}

int VITimerInterruptErrorCheck(char received_address) { // return 1 if invalid address. Invalid if parity bit is set to 0
    return (received_address >> 7) != 1;
}

int VITimerCheckContinuationBit(VirtualInterruptFrame *frame_buffer) {
    return (frame_buffer->interrupt_address >> 6) ^ 1 == 0;
}

void VISetGRFHigh() {
    global_reading_flag = 1;
}

void VISetGRFLow() {
    global_reading_flag = 0;
}