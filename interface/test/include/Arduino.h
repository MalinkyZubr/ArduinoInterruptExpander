#ifndef ARDUINO_H
#define ARDUINO_H

#define INPUT_PULLUP 0
#define INT0_vect 0
#define INT1_vect 1
#define OUTPUT 0
#define INPUT 1
#define LOW 0
#define HIGH 1
#define ISR(vector)

void sei();
void cli();
void pinMode(int pin, int mode);
void digitalWrite(int pin, int state);

#endif