// remember to do SPI.begin()
#include "../include/VirtualInterruptBoard.hpp"
#include <Arduino.h>
#include <SPI.h>


int execution_count = 0;

void integrationTestISR() {
    execution_count++;
}

int main() {
    SPI.begin();
    VI_Manager.attachVIInterrupt(VIRTUAL_INTERRUPT_17, integrationTestISR, 0);

    while(execution_count < 10) {
        VI_Manager.runTaskFromQueue();
    }
}