#include "./include/test.h"


TestSpace test_space;

void testISR(){
    test_space.execution_count++;
    fprintf(stderr, "\nINCREMENTING EXECUTION COUNT %d", test_space.execution_count);
}