#ifndef VIRTUALINTERRUPTTASKQUEUE_HPP
#define VIRTUALINTERRUPTTASKQUEUE_HPP

#include <stdlib.h>
#include "VirtualInterrupt.h"


typedef struct VITask {
    VirtualISR isr;
    VITask *previous;
} VITask;


class VITaskQueue {
    private:
    VITask *head = nullptr;
    VITask *rear = nullptr;
    int num_tasks;

    public:
    void execute_task();
    void push_task(VirtualISR isr);

    int get_num_tasks();
};


#endif