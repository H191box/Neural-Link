#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include "gba_types.h"

/* Menu items */
typedef enum {
    MENU_NEW_GAME = 0,
    MENU_CONTINUE,
    MENU_GALLERY,
    MENU_SETTINGS,
    MENU_COUNT
} MenuItem;

/* Initialize title screen */
void title_screen_init(void);

/* Update title screen */
void title_screen_update(void);

/* Render title screen */
void title_screen_render(void);

#endif /* TITLE_SCREEN_H */