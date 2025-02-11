#include "software_timer.h"

bool timer_flag = false;
int timer_counter = 0;
Ticker timer;

void setTimer(int duration)
{
    timer_flag = false;
    timer_counter = duration;
    timer.attach_ms(duration,timer_interrupt);
}

void timer_interrupt()
{
  timer_flag = true;
}