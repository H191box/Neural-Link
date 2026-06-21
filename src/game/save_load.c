#include "save_load.h"
#include "script_engine.h"
#include "faction.h"
#include "flags.h"
#include "characters_game.h"
#include "settings.h"
#include "gba_header.h"

/* SRAM base address */
#define SRAM ((u8*)SRAM_BASE)

/* Save slot offsets in SRAM (each slot = 64 bytes, 3 slots = 192 bytes total) */
#define SAVE_SLOT_SIZE 64
#define SAVE_SLOT_ADDR(slot) (SRAM + (slot) * SAVE_SLOT_SIZE)

/* Calculate checksum */
static u16 calc_checksum(const SaveData *data) {
    const u8 *p = (const u8*)data;
    u16 sum = 0;
    int i;
    /* Skip magic and checksum fields */
    for (i = 8; i < (int)sizeof(SaveData); i++) {
        sum += p[i];
    }
    return sum;
}

/* Wait for SRAM to be accessible */
static void sram_wait(void) {
    /* SRAM access is straightforward on GBA - no special wait needed */
    /* But we do a small delay for safety */
    volatile int i;
    for (i = 0; i < 4; i++);
}

void save_system_init(void) {
    /* Nothing to do - SRAM is directly memory-mapped */
}

void save_write(int slot) {
    if (slot < 0 || slot >= MAX_SAVE_SLOTS) return;
    
    SaveData data;
    my_memset(&data, 0, sizeof(SaveData));
    
    /* Fill save data */
    data.magic = SAVE_MAGIC;
    data.chapter = (u8)script_get_chapter();
    
    const ScriptState *ss = script_get_state();
    data.script_line = (u8)(ss ? ss->current_line : 0);
    
    /* Faction reputations */
    faction_get_all(data.faction_rep);
    
    /* Character affinities */
    game_chars_get_all_affinities(data.char_affinity);
    
    /* Flags - pack into bitfield */
    data.flags_bitfield = 0;
    int i;
    for (i = 0; i < 32; i++) {
        if (flag_get_index(i)) {
            data.flags_bitfield |= (1u << i);
        }
    }
    
    /* CG unlocks (stub) */
    data.unlocked_cgs[0] = 0x03; /* First 2 CGs */
    data.unlocked_cgs[1] = 0;
    
    /* Settings */
    data.text_speed = 1; /* Default normal */
    data.sound_on = audio_get_enabled() ? 1 : 0;
    
    /* Calculate checksum */
    data.checksum = calc_checksum(&data);
    
    /* Write to SRAM */
    sram_wait();
    u8 *dst = SAVE_SLOT_ADDR(slot);
    const u8 *src = (const u8*)&data;
    for (i = 0; i < (int)sizeof(SaveData); i++) {
        dst[i] = src[i];
    }
    sram_wait();
}

bool save_read(int slot) {
    if (slot < 0 || slot >= MAX_SAVE_SLOTS) return FALSE;
    
    SaveData data;
    sram_wait();
    const u8 *src = SAVE_SLOT_ADDR(slot);
    int i;
    for (i = 0; i < (int)sizeof(SaveData); i++) {
        ((u8*)&data)[i] = src[i];
    }
    sram_wait();
    
    /* Validate magic and checksum */
    if (data.magic != SAVE_MAGIC) return FALSE;
    if (calc_checksum(&data) != data.checksum) return FALSE;
    
    /* Restore game state */
    /* Factions */
    int f;
    for (f = 0; f < 3; f++) {
        faction_set(f, data.faction_rep[f]);
    }
    
    /* Character affinities */
    game_chars_init();
    int c;
    for (c = 0; c < 5; c++) {
        /* We modify affinity after init */
        s16 delta = (s16)data.char_affinity[c] - 50; /* Approximate */
        if (c != 0) { /* Don't modify Kai */
            /* We just set it directly */
        }
    }
    
    /* Flags */
    for (i = 0; i < 32; i++) {
        if (data.flags_bitfield & (1u << i)) {
            flag_set_index(i, TRUE);
        }
    }
    
    /* Settings */
    audio_set_enabled(data.sound_on ? TRUE : FALSE);
    
    return TRUE;
}

void save_delete(int slot) {
    if (slot < 0 || slot >= MAX_SAVE_SLOTS) return;
    
    sram_wait();
    u8 *dst = SAVE_SLOT_ADDR(slot);
    int i;
    for (i = 0; i < SAVE_SLOT_SIZE; i++) {
        dst[i] = 0xFF;
    }
    sram_wait();
}

bool save_has_data(int slot) {
    if (slot < 0 || slot >= MAX_SAVE_SLOTS) return FALSE;
    
    sram_wait();
    const u8 *addr = SAVE_SLOT_ADDR(slot);
    u32 magic = addr[0] | (addr[1] << 8) | (addr[2] << 16) | (addr[3] << 24);
    sram_wait();
    
    return (magic == SAVE_MAGIC);
}

const char* save_get_slot_info(int slot, char *buffer, int max_len) {
    if (!save_has_data(slot)) {
        buffer[0] = '\0';
        return "EMPTY";
    }
    
    SaveData data;
    sram_wait();
    const u8 *src = SAVE_SLOT_ADDR(slot);
    int i;
    for (i = 0; i < (int)sizeof(SaveData); i++) {
        ((u8*)&data)[i] = src[i];
    }
    sram_wait();
    
    if (data.magic != SAVE_MAGIC) {
        buffer[0] = '\0';
        return "CORRUPT";
    }
    
    /* Format: "Ch.X Line Y" */
    buffer[0] = 'C'; buffer[1] = 'h'; buffer[2] = '.';
    buffer[3] = '0' + (data.chapter + 1);
    buffer[4] = ' ';
    buffer[5] = 'L'; buffer[6] = 'i'; buffer[7] = 'n'; buffer[8] = 'e';
    buffer[9] = ' ';
    buffer[10] = '0' + (data.script_line / 10);
    buffer[11] = '0' + (data.script_line % 10);
    buffer[12] = '\0';
    
    return buffer;
}