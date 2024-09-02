#include "../include/VirtualInterruptTaskQueue.hpp" // configure interrupt to be active low enable


void VITaskQueue::push_task(VirtualISR isr) {
    VITask* new_task = (VITask*)malloc(sizeof(VITask));
    new_task->isr = isr;

    if(this->head == NULL) {
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
    if(this->head != NULL) {
        VITask* selected_task = this->head;

        if(selected_task == this->rear) {
            this->rear = NULL;
            this->head = NULL;
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

VITaskQueue::~VITaskQueue() {
    VITask* selected_task = this->head;
    VITask* next_task = nullptr;
    while(selected_task != NULL) {
        next_task = selected_task->previous;
        free(selected_task);
        selected_task = next_task;
    }
}