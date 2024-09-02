#ifndef TEST_H
#define TEST_H

#include "../include/VirtualInterrupt.hpp"
#include "../include/VirtualInterruptManager.hpp"
#include "../include/VirtualInterruptTaskQueue.hpp"
#include "./SPI.h"
#include "./io.h"
#include "./Arduino.h"


typedef struct TestSpace {
    VirtualInterrupt test_interrupt;
    SPISettings test_settings;
    const uint8_t send_byte = 0b11111111;

    VITaskQueue test_queue;
    VirtualInterruptManager test_manager;

    int execution_count = 0;
} TestSpace;

extern TestSpace test_space;

inline void reset_test_space() {
    test_space.test_queue = VITaskQueue();
    test_space.test_manager = VirtualInterruptManager(&test_space.test_queue);
    test_space.execution_count = 0;
};

void testISR();

#endif