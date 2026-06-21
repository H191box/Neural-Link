#ifndef CHARACTERS_GAME_H
#define CHARACTERS_GAME_H

#include "gba_types.h"

/* Heart event threshold definition per LI */
typedef struct {
    u8 chapter;        /* 0-indexed chapter when event can trigger */
    u8 affinity_req;   /* Minimum affinity required */
} HeartThreshold;

/* Character definitions for game logic (otome version) */
typedef struct {
    const char *name;
    const char *full_name;     /* Full name for formal scenes */
    u8 faction;                /* FACTION_* ID */
    u8 affinity;               /* 0-100, how close to player */
    u8 portrait_id;            /* Sprite palette index */
    u8 text_color;             /* Default text color palette for dialog */
    u8 traits;                 /* Personality trait bitmask */
    u8 preferred_gift;         /* Primary preferred gift type */
    u8 secondary_gift;         /* Secondary preferred gift type */
    u8 disliked_gift;          /* Disliked gift type */
    const char *gift_liked_msg;    /* Message when given preferred gift */
    const char *gift_neutral_msg;  /* Message when given neutral gift */
    const char *gift_disliked_msg; /* Message when given disliked gift */
    HeartThreshold heart_thresholds[5]; /* 4 heart events + confession */
} GameCharacter;

/* Character IDs */
#define GC_KIRA  0   /* Protagonist (female) */
#define GC_NOVA  1   /* LI: The Rebel */
#define GC_CHEN  2   /* LI: The Powerbroker */
#define GC_ARIA  3   /* LI: The AI */
#define GC_VEX   4   /* LI: The Scientist */
#define GC_COUNT 5

/* Initialize character system (otome version) */
void game_chars_init(void);

/* Modify affinity with a character */
void game_chars_modify_affinity(int char_id, s16 delta);

/* Get affinity with a character (0-100) */
u8 game_chars_get_affinity(int char_id);

/* Get character data */
const GameCharacter* game_chars_get(int char_id);

/* Get all affinities */
void game_chars_get_all_affinities(u8 affinities[GC_COUNT]);

/* Check if a character likes a specific gift type (returns affinity delta) */
s8 game_chars_gift_preference(int char_id, int gift_type);

/* Get the gift reaction message for a character and preference level */
const char* game_chars_gift_reaction_msg(int char_id, int gift_type);

/* Get the LI index for a route (1-4), or -1 for common route */
int game_chars_get_li_index(int route_id);

#endif /* CHARACTERS_GAME_H */