#include "save_load.h"
#include "script_engine.h"
#include "faction.h"
#include "flags.h"
#include "characters_game.h"
#include "settings.h"
#include "gift_system.h"
#include "route_system.h"
#include "gba_header.h"

/* SRAM base address */
#define SRAM ((u8*)SRAM_BASE)

/* Save slot offsets in SRAM (each slot = 128 bytes, 3 slots = 384 bytes total) */
/* Increased from 64 to 128 for otome data (gift inventory, route, heart events) */
#define SAVE_SLOT_SIZE 128
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

    /* Faction reputations (first 3 — save is sized for old 3-faction compat) */
    {
        u8 rep_all[FACTION_COUNT];
        faction_get_all(rep_all);
        data.faction_rep[0] = rep_all[FACTION_RESISTANCE];
        data.faction_rep[1] = rep_all[FACTION_MEGACORP];
        data.faction_rep[2] = rep_all[FACTION_NEUTRALS];
        /* Store extra factions in char_affinity padding */
        data.faction_rep_extra[0] = rep_all[FACTION_NEUTRAL_SCIENTISTS];
        data.faction_rep_extra[1] = rep_all[FACTION_AI_COLLECTIVE];
    }

    /* Character affinities */
    game_chars_get_all_affinities(data.char_affinity);

    /* Flags - pack into bitfield */
    data.flags_bitfield = 0;
    {
        int i;
        for (i = 0; i < 32; i++) {
            if (flag_get_index(i)) {
                data.flags_bitfield |= (1u << i);
            }
        }
    }

    /* Gift inventory */
    gift_get_inventory(data.gift_inventory);

    /* Current route */
    data.current_route = (u8)route_get_current();

    /* CG unlocks (stub) */
    data.unlocked_cgs[0] = 0x03;
    data.unlocked_cgs[1] = 0;

    /* Settings */
    data.text_speed = 1;
    data.sound_on = audio_get_enabled() ? 1 : 0;

    /* Calculate checksum */
    data.checksum = calc_checksum(&data);

    /* Write to SRAM */
    sram_wait();
    u8 *dst = SAVE_SLOT_ADDR(slot);
    const u8 *src = (const u8*)&data;
    int i;
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
    {
        u8 rep_all[FACTION_COUNT];
        rep_all[FACTION_RESISTANCE] = data.faction_rep[0];
        rep_all[FACTION_MEGACORP] = data.faction_rep[1];
        rep_all[FACTION_NEUTRALS] = data.faction_rep[2];
        rep_all[FACTION_NEUTRAL_SCIENTISTS] = data.faction_rep_extra[0];
        rep_all[FACTION_AI_COLLECTIVE] = data.faction_rep_extra[1];
        int f;
        for (f = 0; f < FACTION_COUNT; f++) {
            faction_set(f, rep_all[f]);
        }
    }

    /* Character affinities — init then override */
    game_chars_init();
    {
        int c;
        for (c = LI_START; c < GC_COUNT; c++) {
            /* Set affinity directly by modifying repeatedly to reach target */
            /* Since init sets defaults, we need to adjust delta */
            const GameCharacter *ch = game_chars_get(c);
            s16 target = (s16)data.char_affinity[c];
            s16 current = (s16)ch->affinity; /* This is the init default */
            s16 delta = target - current;
            if (delta != 0) {
                game_chars_modify_affinity(c, delta);
            }
        }
    }

    /* Flags */
    for (i = 0; i < 32; i++) {
        if (data.flags_bitfield & (1u << i)) {
            flag_set_index(i, TRUE);
        }
    }

    /* Gift inventory */
    gift_init();
    gift_set_inventory(data.gift_inventory);

    /* Route */
    if (data.current_route >= ROUTE_NOVA && data.current_route <= ROUTE_VEX) {
        route_lock_in(data.current_route);
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

    /* Format: "Ch.X Li:Name" or "Ch.X Common" */
    buffer[0] = 'C'; buffer[1] = 'h'; buffer[2] = '.';
    buffer[3] = '0' + (data.chapter + 1);
    buffer[4] = ' ';

    if (data.current_route >= ROUTE_NOVA && data.current_route <= ROUTE_VEX) {
        const char *li_names[] = {"", "Nova", "Chen", "ARIA", "Vex"};
        const char *name = li_names[data.current_route];
        int j = 5;
        while (*name && j < max_len - 1) {
            buffer[j++] = *name++;
        }
        buffer[j] = '\0';
    } else {
        buffer[5] = 'C'; buffer[6] = 'o'; buffer[7] = 'm';
        buffer[8] = 'm'; buffer[9] = 'o'; buffer[10] = 'n';
        buffer[11] = '\0';
    }

    return buffer;
}