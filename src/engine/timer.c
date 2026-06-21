#include "timer.h"

void timer_init(int tm, u16 freq_div, bool irq) {
    volatile u16 *cnt = NULL;
    switch (tm) {
        case 0: cnt = &REG_TM0CNT; break;
        case 1: cnt = &REG_TM1CNT; break;
        case 2: cnt = &REG_TM2CNT; break;
        case 3: cnt = &REG_TM3CNT; break;
    }
    if (cnt) *cnt = freq_div | (irq ? TIMER_IRQ : 0);
}

void timer_start(int tm, u16 value) {
    volatile u16 *data = NULL;
    volatile u16 *cnt = NULL;
    switch (tm) {
        case 0: data = &REG_TM0D; cnt = &REG_TM0CNT; break;
        case 1: data = &REG_TM1D; cnt = &REG_TM1CNT; break;
        case 2: data = &REG_TM2D; cnt = &REG_TM2CNT; break;
        case 3: data = &REG_TM3D; cnt = &REG_TM3CNT; break;
    }
    if (data) *data = value;
    if (cnt) *cnt |= TIMER_ENABLE;
}

void timer_stop(int tm) {
    volatile u16 *cnt = NULL;
    switch (tm) {
        case 0: cnt = &REG_TM0CNT; break;
        case 1: cnt = &REG_TM1CNT; break;
        case 2: cnt = &REG_TM2CNT; break;
        case 3: cnt = &REG_TM3CNT; break;
    }
    if (cnt) *cnt &= ~TIMER_ENABLE;
}

u16 timer_read(int tm) {
    switch (tm) {
        case 0: return REG_TM0D;
        case 1: return REG_TM1D;
        case 2: return REG_TM2D;
        case 3: return REG_TM3D;
    }
    return 0;
}

void timer_cascade(int tm0, int tm1) {
    (void)tm0;
    volatile u16 *cnt = NULL;
    switch (tm1) {
        case 1: cnt = &REG_TM1CNT; break;
        case 2: cnt = &REG_TM2CNT; break;
        case 3: cnt = &REG_TM3CNT; break;
    }
    if (cnt) *cnt |= TIMER_CASCADE;
}

/* Simple millisecond timer: TM0 at freq/64, TM1 cascade */
static u32 ms_base = 0;

void timer_ms_init(void) {
    timer_init(0, TIMER_FREQ_64, FALSE);
    timer_init(1, 0, FALSE);
    timer_cascade(0, 1);
    REG_TM0D = 0;
    REG_TM1D = 0;
    REG_TM0CNT |= TIMER_ENABLE;
    REG_TM1CNT |= TIMER_ENABLE;
    ms_base = 0;
}

u32 timer_ms_get(void) {
    u32 t = (u32)(65536 - REG_TM0D) * 65536 + (65536 - REG_TM1D);
    /* Convert from timer ticks to approximate ms */
    /* At freq/64: tick = 64/16.78MHz ≈ 3.814us, so 65536 ticks = 250ms */
    return (t * 250) / 65536;
}

void timer_ms_reset(void) {
    ms_base = timer_ms_get();
}