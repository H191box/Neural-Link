#include "characters_game.h"
#include "faction.h"
#include "gba_types.h"
#include "text_renderer.h"

static GameCharacter chars[GC_COUNT];
static u8 affinities[GC_COUNT];

void game_chars_init(void) {
    /* Kai - player character, neural engineer */
    chars[GC_KAI].name = "Kai";
    chars[GC_KAI].faction = FACTION_NEUTRALS;
    chars[GC_KAI].affinity = 100;
    chars[GC_KAI].portrait_id = CHAR_KAI;
    chars[GC_KAI].text_color = TEXT_COLOR_WHITE;
    
    /* Nova - resistance leader */
    chars[GC_NOVA].name = "Nova";
    chars[GC_NOVA].faction = FACTION_RESISTANCE;
    chars[GC_NOVA].affinity = 40;
    chars[GC_NOVA].portrait_id = CHAR_NOVA;
    chars[GC_NOVA].text_color = TEXT_COLOR_GREEN;
    
    /* Director Chen - MegaCorp executive */
    chars[GC_CHEN].name = "Chen";
    chars[GC_CHEN].faction = FACTION_MEGACORP;
    chars[GC_CHEN].affinity = 30;
    chars[GC_CHEN].portrait_id = CHAR_CHEN;
    chars[GC_CHEN].text_color = TEXT_COLOR_RED;
    
    /* ARIA - AI assistant */
    chars[GC_ARIA].name = "ARIA";
    chars[GC_ARIA].faction = FACTION_NEUTRALS;
    chars[GC_ARIA].affinity = 50;
    chars[GC_ARIA].portrait_id = CHAR_ARIA;
    chars[GC_ARIA].text_color = TEXT_COLOR_CYAN;
    
    /* Dr. Vex - mysterious scientist */
    chars[GC_VEX].name = "Dr. Vex";
    chars[GC_VEX].faction = FACTION_NEUTRALS;
    chars[GC_VEX].affinity = 20;
    chars[GC_VEX].portrait_id = CHAR_VEX;
    chars[GC_VEX].text_color = TEXT_COLOR_ORANGE;
    
    /* Init affinities */
    int i;
    for (i = 0; i < GC_COUNT; i++) {
        affinities[i] = chars[i].affinity;
    }
}

void game_chars_modify_affinity(int char_id, s16 delta) {
    if (char_id < 0 || char_id >= GC_COUNT) return;
    s16 val = (s16)affinities[char_id] + delta;
    affinities[char_id] = (u8)CLAMP(val, 0, 100);
}

u8 game_chars_get_affinity(int char_id) {
    if (char_id < 0 || char_id >= GC_COUNT) return 0;
    return affinities[char_id];
}

const GameCharacter* game_chars_get(int char_id) {
    if (char_id < 0 || char_id >= GC_COUNT) return NULL;
    return &chars[char_id];
}

void game_chars_get_all_affinities(u8 a[GC_COUNT]) {
    int i;
    for (i = 0; i < GC_COUNT; i++) {
        a[i] = affinities[i];
    }
}