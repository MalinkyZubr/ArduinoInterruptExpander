#include "../include/VirtualInterruptBoard.hpp"


VITaskQueue task_queue = VITaskQueue();
VirtualInterruptManager VI_Manager = VirtualInterruptManager(&task_queue);


