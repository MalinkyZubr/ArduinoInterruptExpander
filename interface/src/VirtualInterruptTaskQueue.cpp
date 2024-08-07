#include "../include/VirtualInterruptTaskQueue.hpp"


void VITaskQueue::push_task(VirtualISR isr) {
    VITask* new_task = (VITask*)malloc(sizeof(VITask));
    new_task->isr = isr;

    if(this->head == nullptr) {
        this->head = new_task;
        this->rear = new_task;
    }
    else {
        this->rear->previous = new_task;
        this->rear = new_task;
    }
    this->num_tasks++;
}

void VITaskQueue::execute_task() {
    if(this->head != nullptr) {
        VITask* selected_task = this->head;

        if(selected_task == this->rear) {
            this->rear = nullptr;
            this->head = nullptr;
        }
        else {
            this->head = selected_task->previous;
        }

        selected_task->isr();

        free(selected_task);
        this->num_tasks--;
    }
}

int VITaskQueue::get_num_tasks() {
    return this->num_tasks;
}