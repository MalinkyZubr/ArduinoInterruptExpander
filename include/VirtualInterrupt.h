#ifndef VIRTUALINTERRUPT_H
#define VIRTUALINTERRUPT_H


#define VI_MAXIMUM_ADDRESS 0b00011111
#define VI_MAXIMUM_BITS 7
#define VI_MAXIMUM_DEVICES 32


typedef struct VirtualInterruptFrame {
    int interrupt_address = 0b00000000;
    int continuation_bit = 0;
    int bits_received = 0;
} VirtualInterruptFrame;

typedef void (*VirtualISR)();

enum InterruptAddress {
    DEVICE_01 = 0b00000000,
    DEVICE_02 = 0b00000001,
    DEVICE_03 = 0b00000010,
    DEVICE_04 = 0b00000011,
    DEVICE_05 = 0b00000100,
    DEVICE_06 = 0b00000101,
    DEVICE_07 = 0b00000110,
    DEVICE_08 = 0b00000111,
    DEVICE_09 = 0b00001000,
    DEVICE_10 = 0b00001001,
    DEVICE_11 = 0b00001010,
    DEVICE_12 = 0b00001011,
    DEVICE_13 = 0b00001100,
    DEVICE_14 = 0b00001101,
    DEVICE_15 = 0b00001110,
    DEVICE_16 = 0b00001111,
    DEVICE_17 = 0b00010000,
    DEVICE_18 = 0b00010001,
    DEVICE_19 = 0b00010010,
    DEVICE_20 = 0b00010011,
    DEVICE_21 = 0b00010100,
    DEVICE_22 = 0b00010101,
    DEVICE_23 = 0b00010110,
    DEVICE_24 = 0b00010111,
    DEVICE_25 = 0b00011000,
    DEVICE_26 = 0b00011001,
    DEVICE_27 = 0b00011010,
    DEVICE_28 = 0b00011011,
    DEVICE_29 = 0b00011100,
    DEVICE_30 = 0b00011101,
    DEVICE_31 = 0b00011110,
    DEVICE_32 = 0b00011111
};

typedef struct VirtualInterrupt {
    int initialized = 0;
    int immutable = 0;
    int enabled = 0;
    enum InterruptAddress address;
    VirtualISR isr;
} VirtualInterrupt;

VirtualInterrupt instantiate_interrupt(enum InterruptAddress address);

void ReceiveTriggerBit();

void ReceiveVIBit(VirtualInterruptFrame *buffer_frame);

#endif

