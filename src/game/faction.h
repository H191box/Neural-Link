#ifndef FACTION_H
#define FACTION_H

#include "gba_types.h"

/* Faction IDs */
#define FACTION_RESISTANCE 0
#define FACTION_MEGACORP   1
#define FACTION_NEUTRALS   2
#define FACTION_COUNT      3

/* Initialize faction system */
void faction_init(void);

/* Modify reputation with a faction */
void faction_modify(int faction_id, s16 delta);

/* Get reputation with a faction (0-100) */
u8 faction_get(int faction_id);

/* Set reputation directly */
void faction_set(int faction_id, u8 value);

/* Get the faction with highest reputation */
int faction_get_highest(void);

/* Get all reputations as array */
void faction_get_all(u8 rep[FACTION_COUNT]);

/* Reset all reputations */
void faction_reset(void);

#endif /* FACTION_H */