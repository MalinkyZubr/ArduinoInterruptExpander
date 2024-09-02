#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/VirtualInterrupt.hpp"
#include "../include/VirtualInterruptManager.hpp"
#include "../include/VirtualInterruptTaskQueue.hpp"
#include "./include/SPI.h"
#include "./include/io.h"
#include "./include/Arduino.h"
#include "./include/doctest.h"
#include "./include/test.h"


TEST_CASE("Testing SPI Interface Subclasses") {
    reset_test_space();

    SUBCASE("testing virtual interrupt") {
        test_space.test_interrupt = instantiate_interrupt(VIRTUAL_INTERRUPT_01);
        CHECK(test_space.test_interrupt.initialized == 0);
        CHECK(test_space.test_interrupt.enabled == 0);
        CHECK(test_space.test_interrupt.immutable == 0);
        CHECK(test_space.test_interrupt.address == VIRTUAL_INTERRUPT_01);
    }

    SUBCASE("Checking to see if VIRead functions properly") {
        CHECK(VIRead(test_space.send_byte, test_space.test_settings) == 0b00000000);
    }

    SUBCASE("Checking to see if basic Queue operations work properly") {
        for(int x = 0; x < 3; x++) {
            test_space.test_queue.push_task(testISR);
        }
        CHECK(test_space.test_queue.get_num_tasks() == 3);
        test_space.test_queue.execute_task();
        CHECK(test_space.test_queue.get_num_tasks() == 2);
        CHECK(test_space.execution_count == 1);

        test_space.test_queue.execute_task();
        test_space.test_queue.execute_task();

        CHECK(test_space.execution_count == 3);
        test_space.test_queue.execute_task();
        CHECK(test_space.test_queue.get_num_tasks() == 0);
    }
}


TEST_CASE("Testing the main SPI interrupt expander framework") {
    reset_test_space();
}
