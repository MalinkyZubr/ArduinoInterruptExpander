#include "../include/VirtualInterruptManager.hpp"


VirtualInterruptManager::VirtualInterruptManager(TriggerInteruptPin interrupt_pin) : interrupt_pin(interrupt_pin) {
    for(int interrupt_address = 0; interrupt_address < VI_MAXIMUM_DEVICES; interrupt_address++) {
        this->interrupt_table[interrupt_address] = instantiate_interrupt((InterruptAddress) interrupt_address);
    }
}

void VirtualInterruptManager::enable_input_trigger() {
    attachInterrupt(digitalPinToInterrupt(this->interrupt_pin), ReceiveTriggerBit, RISING);
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