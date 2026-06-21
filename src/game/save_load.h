#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "gba_types.h"

#define MAX_SAVE_SLOTS 3

/* Save data structure (must be compact for SRAM) */
typedef struct {
    u32 magic;           /* Magic number for validation */
    u16 checksum;        /* Simple checksum */
    u8 chapter;          /* Current chapter (0-4) */
    u8 script_line;      /* Script line position */
    u8 faction_rep[3];   /* Faction reputations */
    u8 char_affinity[5]; /* Character affinities */
    u32 flags_bitfield;  /* Up to 32 flags as bits */
    u8 unlocked_cgs[2];  /* 16 CG unlock bits */
    u8 text_speed;       /* 0=slow, 1=normal, 2=fast */
    u8 sound_on;         /* 1=on, 0=off */
    u8 padding[16];      /* Future use */
} SaveData;

#define SAVE_MAGIC 0x4E4C5631 /* "NLV1" */

/* Initialize save system */
void save_system_init(void);

/* Write save data to a slot */
void save_write(int slot);

/* Read save data from a slot */
bool save_read(int slot);

/* Delete save data in a slot */
void save_delete(int slot);

/* Check if a slot has save data */
bool save_has_data(int slot);

/* Get save slot info as string */
const char* save_get_slot_info(int slot, char *buffer, int max_len);

#endif /* SAVE_LOAD_H */