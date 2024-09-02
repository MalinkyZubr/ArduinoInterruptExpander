#include "./include/test.h"


TestSpace test_space;

void testISR(){
    test_space.execution_count++;
}