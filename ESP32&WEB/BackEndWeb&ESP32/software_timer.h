#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_
#include <Ticker.h>
#include <Arduino.h>

extern bool timer_flag;
extern Ticker timer;

void timer_interrupt();
void setTimer(int duration);

#endif