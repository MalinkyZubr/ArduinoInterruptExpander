#include "../include/VirtualInterruptManager.hpp"


VirtualInterruptManager::VirtualInterruptManager(VICSPin cs_pin) : cs_pin(cs_pin) {
    pinMode(read_pin, INPUT);
    pinMode(clock_pin, OUTPUT);

    for(int interrupt_address = 0; interrupt_address < VI_MAXIMUM_DEVICES - 1; interrupt_address++) {
        this->interrupt_table[interrupt_address] = instantiate_interrupt((InterruptAddress) interrupt_address);
    }

    this->enable_input_trigger();
    this->clock_manager.clock_pin = clock_pin
}

void VirtualInterruptManager::enable_input_trigger() {
    attachInterrupt(digitalPinToInterrupt(this->interrupt_pin), VISetGRFHigh(), RISING);
}

void VirtualInterruptManager::disable_input_trigger() {
    detachInterrupt(digitalPinToInterrupt(this->interrupt_pin));
}

VIManagerReturn VirtualInterruptManager::attachVIInterrupt(InterruptAddress interrupt_address, VirtualISR isr, int immutable = 0) {
    VirtualInterrupt* virtual_interrupt = &this->interrupt_table[interrupt_address];
    VIManagerReturn return_value;

    if(virtual_interrupt->initialized == 0) {
        virtual_interrupt->initialized = 1;
        virtual_interrupt->isr = isr;
        virtual_interrupt->immutable = immutable;

        return_value = VI_OP_SUCCESS;
    }
    else {
        return_value = VI_ADDRESS_OCCUPIED;
    }

    return return_value;
}

VIManagerReturn VirtualInterruptManager::modifyVIInterrupt(InterruptAddress interrupt_address, VirtualISR isr) {
    VirtualInterrupt* virtual_interrupt = &this->interrupt_table[interrupt_address];
    VIManagerReturn return_value;

    if(virtual_interrupt->immutable == 0) {
        if(virtual_interrupt->initialized == 1) {
            virtual_interrupt->isr = isr;
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
    VirtualInterrupt* virtual_interrupt = &this->interrupt_table[interrupt_address];
    VIManagerReturn return_value;

    if(virtual_interrupt->initialized == 1) {
        this->interrupt_table[interrupt_address] = instantiate_interrupt((InterruptAddress) interrupt_address);
        return_value = VI_OP_SUCCESS;
    }
    else {
        return_value = VI_ADDRESS_NOT_LOADED;
    }
}

void VirtualInterruptManager::enableVIInterrupt(InterruptAddress interrupt_address) {
    VirtualInterrupt* virtual_interrupt = &this->interrupt_table[interrupt_address];
    virtual_interrupt->enabled = 1;
}

void VirtualInterruptManager::disableVIInterrupt(InterruptAddress interrupt_address) {
    VirtualInterrupt* virtual_interrupt = &this->interrupt_table[interrupt_address];
    virtual_interrupt->enabled = 0;
}

void VirtualInterruptManager::triggerVIInterrupt(InterruptAddress interrupt_address) {
    VirtualInterrupt* virtual_interrupt = &this->interrupt_table[interrupt_address];
    VIManagerReturn return_value;

    if(virtual_interrupt->initialized == 1) {
        if(virtual_interrupt->enabled == 1) {
            virtual_interrupt->isr();
            return_value = VI_OP_SUCCESS;
        }
        else {
            return_value = VI_DISABLED;
        }
    }
    else {
        return_value = VI_ADDRESS_NOT_LOADED;
    }
}

VIReadPin VirtualInterruptManager::get_read_pin() {
    return this->read_pin;
}

VIClockManager* VirtualInterruptManager::get_clock_manager() {
    return &this->clock_manager;
}