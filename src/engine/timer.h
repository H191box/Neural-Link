#ifndef TIMER_H
#define TIMER_H

#include "gba_types.h"
#include "gba_header.h"

/* Initialize a timer (0-3) with frequency divider */
void timer_init(int tm, u16 freq_div, bool irq);

/* Start a timer with initial value */
void timer_start(int tm, u16 value);

/* Stop a timer */
void timer_stop(int tm);

/* Read current timer value */
u16 timer_read(int tm);

/* Set up timer cascade: tm1 counts when tm0 overflows */
void timer_cascade(int tm0, int tm1);

/* Simple millisecond timer using cascade */
void timer_ms_init(void);
u32 timer_ms_get(void);
void timer_ms_reset(void);

#endif /* TIMER_H */