#ifndef TIMER_H
#define TIMER_H

#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL0_DATA_PORT 0x40
#define PIT_BASE_FREQUENCY 1193182

void timer_init(int frequency);

#endif