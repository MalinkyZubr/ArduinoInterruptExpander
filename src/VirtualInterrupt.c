#include "../include/VirtualInterrupt.h"


VirtualInterrupt instantiate_interrupt(enum InterruptAddress interrupt_address) {
    VirtualInterrupt interrupt;
    interrupt.address = interrupt_address;
    return interrupt;
}