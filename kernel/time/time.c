#include "time.h"
#include "../../drivers/screen/screen.h"

extern volatile unsigned int timer_ticks;

void sleep(unsigned int milliseconds)
{
    unsigned int ticks_to_wait = milliseconds / 10; // since 100Hz is the frequency of the ticker
    unsigned int curr_ticker = timer_ticks;
    while (timer_ticks < curr_ticker + ticks_to_wait)
    {
        __asm__ __volatile__("hlt");
    }
}