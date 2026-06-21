#ifndef NOVEL_STATE_H
#define NOVEL_STATE_H

#include "gba_types.h"

/* Game states — extended for otome systems */
typedef enum {
    STATE_TITLE = 0,
    STATE_CHAPTER_INTRO,
    STATE_DIALOG,
    STATE_CHOICE,
    STATE_HACKING,
    STATE_GIFT_SELECT,      /* NEW: Gift selection screen */
    STATE_HEART_EVENT,      /* NEW: Heart event scene */
    STATE_ROUTE_LOCK,       /* NEW: Route lock choice */
    STATE_CG_GALLERY,
    STATE_SAVE_LOAD,
    STATE_SETTINGS,
    STATE_ENDING
} GameState;

/* Initialize novel state machine */
void novel_state_init(void);

/* Set current state */
void novel_state_set_state(GameState state);

/* Get current state */
GameState novel_state_get_state(void);

/* Update current state */
void novel_state_update(void);

/* Render current state */
void novel_state_render(void);

/* Set chapter title text */
void novel_state_set_chapter_title(const char *title);

/* Get chapter title text */
const char* novel_state_get_chapter_title(void);

/* Start a new game */
void novel_state_new_game(void);

/* Continue from save */
void novel_state_continue_game(int slot);

#endif /* NOVEL_STATE_H */