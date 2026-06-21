#ifndef TRANSITION_H
#define TRANSITION_H

#include "gba_types.h"

/* Transition types */
typedef enum {
    TRANS_NONE = 0,
    TRANS_FADE_OUT,
    TRANS_FADE_IN,
    TRANS_CROSS_DISSOLVE,
    TRANS_WHITE_OUT,
    TRANS_WHITE_IN
} TransType;

/* Transition state */
typedef struct {
    TransType type;
    int frame;
    int duration;     /* Total frames for transition */
    bool active;
    bool done;
    /* For cross-dissolve: previous BG stored here */
    u16 prev_bg[32 * 32];  /* Store BG0 tilemap for dissolve */
} Transition;

/* Initialize transition system */
void trans_init(void);

/* Start a transition */
void trans_start(TransType type, int duration);

/* Update transition (call each frame), returns true when complete */
bool trans_update(void);

/* Check if transition is active */
bool trans_active(void);

/* Check if transition is done */
bool trans_done(void);

/* Get current fade level (0 = none, 16 = full) */
int trans_get_fade_level(void);

/* Apply fade overlay to screen (BLDY register) */
void trans_apply_fade(int level);

/* Fade to black */
void trans_fade_out(int duration);

/* Fade from black */
void trans_fade_in(int duration);

/* Cross dissolve between two scenes (capture current BG, then load new one externally) */
void trans_cross_dissolve_start(int duration);
void trans_cross_dissolve_load_new(void); /* Call after starting, before update */

#endif /* TRANSITION_H */