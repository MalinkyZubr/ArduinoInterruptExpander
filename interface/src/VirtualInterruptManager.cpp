#include "../include/VirtualInterruptManager.hpp"


VirtualInterruptManager::VirtualInterruptManager(VITaskQueue *task_queue) : task_queue(task_queue) {
    pinMode(VI_CS_PIN, INPUT_PULLUP);

    for(int interrupt_address = 0; interrupt_address < VI_MAXIMUM_ADDRESS; interrupt_address++) {
        this->interrupt_table[interrupt_address] = instantiate_interrupt((InterruptAddress) interrupt_address);
    }
}

VIManagerReturn VirtualInterruptManager::isLoadedVI(InterruptAddress interrupt_address) {
    if(this->interrupt_table[interrupt_address].initialized) {
        return VI_ADDRESS_OCCUPIED;
    }
    return VI_ADDRESS_NOT_LOADED;
}

VIManagerReturn VirtualInterruptManager::attachVIInterrupt(InterruptAddress interrupt_address, VirtualISR isr, int immutable = 0) {
    VirtualInterrupt virtual_interrupt = this->interrupt_table[interrupt_address];
    VIManagerReturn return_value;

    if(virtual_interrupt.initialized == 0) {
        virtual_interrupt.initialized = 1;
        virtual_interrupt.enabled = 1;
        virtual_interrupt.isr = isr;
        virtual_interrupt.immutable = immutable;

        this->interrupt_table[interrupt_address] = virtual_interrupt;

        return_value = VI_OP_SUCCESS;
    }
    else {
        return_value = VI_ADDRESS_OCCUPIED;
    }

    return return_value;
}

VirtualInterruptManager* VirtualInterruptManager::interruptTableBuilder(InterruptAddress interrupt_address, VirtualISR isr, int immutable = 0) {
    VIManagerReturn return_value;

    if(this->isLoadedVI(interrupt_address) == VI_ADDRESS_OCCUPIED) {
        this->detachVIInterrupt(interrupt_address);
    }
    this->attachVIInterrupt(interrupt_address, isr, immutable);

    return this;
}

VIManagerReturn VirtualInterruptManager::modifyVIInterrupt(InterruptAddress interrupt_address, VirtualISR isr) {
    VirtualInterrupt virtual_interrupt = this->interrupt_table[interrupt_address];
    VIManagerReturn return_value;

    if(virtual_interrupt.immutable == 0) {
        if(virtual_interrupt.initialized == 1) {
            virtual_interrupt.isr = isr;
            this->interrupt_table[interrupt_address] = virtual_interrupt;

            return_value = VI_OP_SUCCESS;
        }
        else {
            return_value = VI_ADDRESS_NOT_LOADED;
        }
    }
    else {
        return_value = VI_IMMUTABLE;
    }

    return return_value;
}

VIManagerReturn VirtualInterruptManager::detachVIInterrupt(InterruptAddress interrupt_address) {
    VirtualInterrupt virtual_interrupt = this->interrupt_table[interrupt_address];
    VIManagerReturn return_value;

    if(virtual_interrupt.initialized == 1) {
        this->interrupt_table[interrupt_address] = instantiate_interrupt((InterruptAddress) interrupt_address);
        return_value = VI_OP_SUCCESS;
    }
    else {
        return_value = VI_ADDRESS_NOT_LOADED;
    }

    return return_value;
}

VIManagerReturn VirtualInterruptManager::enableVIInterrupt(InterruptAddress interrupt_address) {
    VirtualInterrupt virtual_interrupt = this->interrupt_table[interrupt_address];
    VIManagerReturn return_value;

    if(virtual_interrupt.initialized == 1) {
        virtual_interrupt.enabled = 1;
        this->interrupt_table[interrupt_address] = virtual_interrupt;

        return_value = VI_OP_SUCCESS;
    }
    else {
        return_value = VI_ADDRESS_NOT_LOADED;
    }

    return return_value;
}

VIManagerReturn VirtualInterruptManager::disableVIInterrupt(InterruptAddress interrupt_address) {
    VirtualInterrupt virtual_interrupt = this->interrupt_table[interrupt_address];
    VIManagerReturn return_value;

    if(virtual_interrupt.initialized == 1) {
        virtual_interrupt.enabled = 0;
        this->interrupt_table[interrupt_address] = virtual_interrupt;

        return_value = VI_OP_SUCCESS;
    }
    else {
        return_value = VI_ADDRESS_NOT_LOADED;
    }

    return return_value;
}

VIManagerReturn VirtualInterruptManager::triggerVIInterrupt(InterruptAddress interrupt_address) {
    VirtualInterrupt virtual_interrupt = this->interrupt_table[interrupt_address];
    VirtualISR interrupt_isr = virtual_interrupt.isr;
    VIManagerReturn return_value;

    if(virtual_interrupt.initialized == 1) {
        if(virtual_interrupt.enabled == 1) {
            this->task_queue->push_task(interrupt_isr); // remember, what will happen if some program state is changed when task queue is populated? might cause issues
            return_value = VI_OP_SUCCESS;
        }
        else {
            return_value = VI_DISABLED;
        }
    }
    else {
        return_value = VI_ADDRESS_NOT_LOADED;
    }

    return return_value;
}

VIManagerReturn VirtualInterruptManager::runTaskFromQueue() {
    VIManagerReturn return_value;

    if(!this->task_queue->execute_task()) {
        return_value = VI_OP_SUCCESS;
    }
    else {
        return_value = VI_TASK_QUEUE_EMPTY;
    }

    return return_value;
}


void SPISetup(uint8_t vector) {
    SPI.usingInterrupt(vector);
}