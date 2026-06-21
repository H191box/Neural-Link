#ifndef CHARACTERS_GAME_H
#define CHARACTERS_GAME_H

#include "gba_types.h"

/* Character definitions for game logic */
typedef struct {
    const char *name;
    u8 faction;      /* FACTION_* ID */
    u8 affinity;     /* 0-100, how close to player */
    u8 portrait_id;  /* Sprite palette index */
    u8 text_color;   /* Default text color palette for dialog */
} GameCharacter;

/* Character IDs (matching asset IDs) */
#define GC_KAI   0
#define GC_NOVA  1
#define GC_CHEN  2
#define GC_ARIA  3
#define GC_VEX   4
#define GC_COUNT 5

/* Initialize character system */
void game_chars_init(void);

/* Modify affinity with a character */
void game_chars_modify_affinity(int char_id, s16 delta);

/* Get affinity with a character (0-100) */
u8 game_chars_get_affinity(int char_id);

/* Get character data */
const GameCharacter* game_chars_get(int char_id);

/* Get all affinities */
void game_chars_get_all_affinities(u8 affinities[GC_COUNT]);

#endif /* CHARACTERS_GAME_H */