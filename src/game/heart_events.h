#ifndef HEART_EVENTS_H
#define HEART_EVENTS_H

#include "gba_types.h"

/* Heart event definition */
typedef struct {
    u8 li_id;             /* Character ID (GC_NOVA through GC_VEX) */
    u8 event_index;       /* 0-4 (4 heart events + 1 confession) */
    u8 chapter_required;  /* 0-indexed chapter when event can trigger */
    u8 affinity_required; /* Minimum affinity needed */
    const char *flag_name;    /* Flag set when event is seen */
    const char *label;        /* Script label to jump to */
    const char *description;  /* Short description for UI */
    bool is_cg;               /* Whether this event unlocks a CG */
} HeartEventDef;

/* Initialize heart events system */
void heart_events_init(void);

/* Check all heart events and return the first available one.
 * Returns pointer to event def, or NULL if none available.
 * Checks: not seen, correct chapter, affinity met. */
const HeartEventDef* heart_events_check_any(int current_chapter);

/* Check if a specific LI has an available heart event */
const HeartEventDef* heart_events_check_li(int li_id, int current_chapter);

/* Trigger a heart event: set the seen flag, unlock CG if applicable.
 * Returns TRUE if event was actually triggered (first time). */
bool heart_events_trigger(const HeartEventDef *event);

/* Check if a specific heart event has been seen */
bool heart_events_is_seen(const char *flag_name);

/* Get all heart event definitions for a specific LI */
int heart_events_get_for_li(int li_id, const HeartEventDef **out, int max_out);

/* ---- Heart Event UI ---- */

/* Initialize heart event notification UI */
void heart_events_ui_init(void);

/* Update heart event UI */
void heart_events_ui_update(void);

/* Render heart event UI */
void heart_events_ui_render(void);

/* Check if heart event UI is done */
bool heart_events_ui_is_done(void);

#endif /* HEART_EVENTS_H */