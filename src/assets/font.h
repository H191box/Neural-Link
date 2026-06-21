#ifndef FONT_H
#define FONT_H

#include "gba_types.h"

/* 8x8 monochrome font - 95 printable ASCII chars (0x20 - 0x7E) */
/* Each char is 8 bytes, 1 bit per pixel (MSB = left) */
extern const u8 font_data_8x8[95][8];

/* Load font into text VRAM */
void font_load(void);

#endif /* FONT_H */