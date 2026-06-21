#ifndef PALETTES_H
#define PALETTES_H

#include "gba_types.h"

/* Background palettes (16 sub-palettes, 16 colors each = 256 colors) */
/* Palette 0: Scene background colors */
/* Palette 1: UI dialog box colors */
/* Palette 2-8: Text colors (one color per sub-palette for coloring) */
/* Palette 9-15: Background-specific palettes */
extern const u16 bg_palette[256];

/* Object palettes */
extern const u16 obj_palette[256];

/* Load all palettes to VRAM */
void palettes_load(void);

/* Get palette entry */
u16 pal_bg(u16 index);
u16 pal_obj(u16 index);

#endif /* PALETTES_H */