#include "input.h"

static u16 input_curr = 0;
static u16 input_prev = 0;

void input_init(void) {
    input_curr = 0;
    input_prev = 0;
}

void input_update(void) {
    input_prev = input_curr;
    /* REG_KEYINPUT is active-low, invert for our use */
    input_curr = ~REG_KEYINPUT & 0x03FF;
}

bool input_held(u16 key) {
    return (input_curr & key) != 0;
}

bool input_pressed(u16 key) {
    return (input_curr & key) && !(input_prev & key);
}

bool input_released(u16 key) {
    return !(input_curr & key) && (input_prev & key);
}

u16 input_get_raw(void) {
    return input_curr;
}