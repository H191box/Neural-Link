#ifndef ROUTE_SYSTEM_H
#define ROUTE_SYSTEM_H

#include "gba_types.h"

/* Initialize route system */
void route_init(void);

/* Lock in a specific route (1-4 for LIs) */
void route_lock_in(int route_id);

/* Auto-lock to the LI with highest affinity (if > ROUTE_AUTO_LOCK_MIN) */
void route_auto_lock(void);

/* Get the current route (0=common, 1-4=LI routes) */
int route_get_current(void);

/* Check if the route has been locked */
bool route_is_locked(void);

/* Get the ending type for current route based on affinity:
 * ENDING_GOOD (0), ENDING_NORMAL (1), ENDING_BAD (2) */
int route_get_ending_type(void);

/* Get the LI character ID for the current route (or -1 if common) */
int route_get_current_li(void);

/* ---- Route Lock UI ---- */

/* Initialize route lock choice UI */
void route_lock_ui_init(void);

/* Update route lock UI */
void route_lock_ui_update(void);

/* Render route lock UI */
void route_lock_ui_render(void);

/* Check if route lock UI is done */
bool route_lock_ui_is_done(void);

/* Get the chosen route from UI (1-4) or -1 if cancelled */
int route_lock_ui_get_result(void);

#endif /* ROUTE_SYSTEM_H */