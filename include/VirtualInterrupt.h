#ifndef VIRTUALINTERRUPT_H
#define VIRTUALINTERRUPT_H


#define VI_MAXIMUM_ADDRESS 0b00011111
#define VI_MAXIMUM_DEVICES 32


volatile int global_reading_flag = 0;

typedef struct VirtualInterruptFrame {
    int interrupt_address = 0b00000000;
    int bits_received = 0;
} VirtualInterruptFrame;

typedef void (*VirtualISR)();

enum InterruptAddress { // LSB trigger bit, MSB parity bit, MSB>>1 continuation bit
    DEVICE_01 = 0b10000001,
    DEVICE_02 = 0b10000011,
    DEVICE_03 = 0b10000101,
    DEVICE_04 = 0b10000111,
    DEVICE_05 = 0b10001001,
    DEVICE_06 = 0b10001011,
    DEVICE_07 = 0b10001101,
    DEVICE_08 = 0b10001111,
    DEVICE_09 = 0b10010001,
    DEVICE_10 = 0b10010011,
    DEVICE_11 = 0b10010101,
    DEVICE_12 = 0b10010111,
    DEVICE_13 = 0b10011001,
    DEVICE_14 = 0b10011011,
    DEVICE_15 = 0b10011101,
    DEVICE_16 = 0b10011111,
    DEVICE_17 = 0b10100001,
    DEVICE_18 = 0b10100011,
    DEVICE_19 = 0b10100101,
    DEVICE_20 = 0b10100111,
    DEVICE_21 = 0b10101001,
    DEVICE_22 = 0b10101011,
    DEVICE_23 = 0b10101101,
    DEVICE_24 = 0b10101111,
    DEVICE_25 = 0b10110001,
    DEVICE_26 = 0b10110011,
    DEVICE_27 = 0b10110101,
    DEVICE_28 = 0b10110111,
    DEVICE_29 = 0b10111001,
    DEVICE_30 = 0b10111011,
    DEVICE_31 = 0b10111101,
    DEVICE_32 = 0b10111111
};

typedef struct VirtualInterrupt {
    int initialized = 0;
    int immutable = 0;
    int enabled = 0;
    enum InterruptAddress address;
    VirtualISR isr;
} VirtualInterrupt;

VirtualInterrupt instantiate_interrupt(enum InterruptAddress address);

char VIExternalIntReceiveTriggerBit();

int VITimerInterruptPWM(int clock_pin, int clock_state);
void VITimerInterruptRead(int read_pin, VirtualInterruptFrame* frame_buffer);

int VITimerInterruptErrorCheck(char received_address);
int VITimerCheckContinuationBit(VirtualInterruptFrame* frame_buffer);

#endif

