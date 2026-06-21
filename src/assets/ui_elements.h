#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H

#include "gba_types.h"

/* UI tile indices used by dialog_box.c and other UI code */
/* These go into BG2 charblock */

/* Load UI tiles (dialog box frame, buttons, cursors) to BG2 charblock */
void ui_elements_load(void);

/* UI tile data (referenced by dialog_box.c) */
extern const u16 ui_tiles[32];

#endif /* UI_ELEMENTS_H */