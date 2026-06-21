#ifndef INPUT_H
#define INPUT_H

#include "gba_types.h"
#include "gba_header.h"

/* Initialize input system - call once at startup */
void input_init(void);

/* Update input state - call once per frame */
void input_update(void);

/* Check if key is currently held */
bool input_held(u16 key);

/* Check if key was just pressed this frame */
bool input_pressed(u16 key);

/* Check if key was just released this frame */
bool input_released(u16 key);

/* Get raw keypad state */
u16 input_get_raw(void);

#endif /* INPUT_H */