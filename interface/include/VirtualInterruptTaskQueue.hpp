#ifndef VIRTUALINTERRUPTTASKQUEUE_HPP
#define VIRTUALINTERRUPTTASKQUEUE_HPP

#include <stdlib.h>
#include "VirtualInterrupt.hpp"


typedef struct VITask {
    VirtualISR isr;
    VITask *previous;
} VITask;


class VITaskQueue {
    private:
    VITask *head = NULL;
    VITask *rear = NULL;
    int num_tasks = 0;

    public:
    VITaskQueue() {}
    int execute_task();
    void push_task(VirtualISR isr);

    int get_num_tasks();

    ~VITaskQueue();
};


#endif