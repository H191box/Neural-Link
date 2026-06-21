#include "characters_game.h"
#include "faction.h"
#include "gba_types.h"
#include "text_renderer.h"

static GameCharacter chars[GC_COUNT];
static u8 affinities[GC_COUNT];

void game_chars_init(void) {
    /* Kira - protagonist, female neural engineer */
    chars[GC_KIRA].name = "Kira";
    chars[GC_KIRA].full_name = "Kira Tanaka";
    chars[GC_KIRA].faction = FACTION_MEGACORP;
    chars[GC_KIRA].affinity = 100;
    chars[GC_KIRA].portrait_id = CHAR_KAI;  /* Reuse existing KAI sprite slot */
    chars[GC_KIRA].text_color = TEXT_COLOR_WHITE;
    chars[GC_KIRA].traits = TRAIT_BRAVE | TRAIT_CURIOUS;
    chars[GC_KIRA].preferred_gift = GIFT_DATA;
    chars[GC_KIRA].secondary_gift = GIFT_BOOK;
    chars[GC_KIRA].disliked_gift = GIFT_WEAPON;
    chars[GC_KIRA].gift_liked_msg = "(This looks useful for my research!)";
    chars[GC_KIRA].gift_neutral_msg = "(Thanks, I suppose.)";
    chars[GC_KIRA].gift_disliked_msg = "(I'm an engineer, not a soldier...)";
    /* Kira has no heart events (she's the protagonist) */
    {
        int i;
        for (i = 0; i < 5; i++) {
            chars[GC_KIRA].heart_thresholds[i].chapter = 0;
            chars[GC_KIRA].heart_thresholds[i].affinity_req = 0;
        }
    }

    /* Nova - The Rebel */
    chars[GC_NOVA].name = "Nova";
    chars[GC_NOVA].full_name = "Nova Vasquez";
    chars[GC_NOVA].faction = FACTION_RESISTANCE;
    chars[GC_NOVA].affinity = 30;
    chars[GC_NOVA].portrait_id = CHAR_NOVA;
    chars[GC_NOVA].text_color = TEXT_COLOR_GREEN;
    chars[GC_NOVA].traits = TRAIT_PASSIONATE | TRAIT_BRAVE;
    chars[GC_NOVA].preferred_gift = GIFT_WEAPON;
    chars[GC_NOVA].secondary_gift = GIFT_DATA;
    chars[GC_NOVA].disliked_gift = GIFT_LUXURY;
    chars[GC_NOVA].gift_liked_msg = "A resistance weapon? You really do understand me.";
    chars[GC_NOVA].gift_neutral_msg = "I appreciate the thought, Kira.";
    chars[GC_NOVA].gift_disliked_msg = "Luxury? While people suffer? Keep it.";
    /* Nova's heart event thresholds */
    chars[GC_NOVA].heart_thresholds[0].chapter = 0;  /* Ch1 */
    chars[GC_NOVA].heart_thresholds[0].affinity_req = AFFINITY_HEART1;
    chars[GC_NOVA].heart_thresholds[1].chapter = 1;  /* Ch2 */
    chars[GC_NOVA].heart_thresholds[1].affinity_req = AFFINITY_HEART2;
    chars[GC_NOVA].heart_thresholds[2].chapter = 2;  /* Ch3 */
    chars[GC_NOVA].heart_thresholds[2].affinity_req = AFFINITY_HEART3;
    chars[GC_NOVA].heart_thresholds[3].chapter = 3;  /* Ch4 */
    chars[GC_NOVA].heart_thresholds[3].affinity_req = AFFINITY_HEART4;
    chars[GC_NOVA].heart_thresholds[4].chapter = 4;  /* Ch5 */
    chars[GC_NOVA].heart_thresholds[4].affinity_req = AFFINITY_CONFESSION;

    /* Director Chen - The Powerbroker */
    chars[GC_CHEN].name = "Chen";
    chars[GC_CHEN].full_name = "Director Wei Chen";
    chars[GC_CHEN].faction = FACTION_MEGACORP;
    chars[GC_CHEN].affinity = 25;
    chars[GC_CHEN].portrait_id = CHAR_CHEN;
    chars[GC_CHEN].text_color = TEXT_COLOR_RED;
    chars[GC_CHEN].traits = TRAIT_CONTROLLED | TRAIT_SECRETIVE;
    chars[GC_CHEN].preferred_gift = GIFT_LUXURY;
    chars[GC_CHEN].secondary_gift = GIFT_TECH;
    chars[GC_CHEN].disliked_gift = GIFT_WEAPON;
    chars[GC_CHEN].gift_liked_msg = "Exquisite taste. You know how to make an impression.";
    chars[GC_CHEN].gift_neutral_msg = "How thoughtful. I'll keep this in mind.";
    chars[GC_CHEN].gift_disliked_msg = "Weapons have no place in my office, Kira.";
    /* Chen's heart event thresholds */
    chars[GC_CHEN].heart_thresholds[0].chapter = 0;
    chars[GC_CHEN].heart_thresholds[0].affinity_req = AFFINITY_HEART1;
    chars[GC_CHEN].heart_thresholds[1].chapter = 1;
    chars[GC_CHEN].heart_thresholds[1].affinity_req = AFFINITY_HEART2;
    chars[GC_CHEN].heart_thresholds[2].chapter = 2;
    chars[GC_CHEN].heart_thresholds[2].affinity_req = AFFINITY_HEART3;
    chars[GC_CHEN].heart_thresholds[3].chapter = 3;
    chars[GC_CHEN].heart_thresholds[3].affinity_req = AFFINITY_HEART4;
    chars[GC_CHEN].heart_thresholds[4].chapter = 4;
    chars[GC_CHEN].heart_thresholds[4].affinity_req = AFFINITY_CONFESSION;

    /* ARIA - The AI */
    chars[GC_ARIA].name = "ARIA";
    chars[GC_ARIA].full_name = "Adaptive Reasoning & Intelligence Architecture";
    chars[GC_ARIA].faction = FACTION_AI_COLLECTIVE;
    chars[GC_ARIA].affinity = 40;
    chars[GC_ARIA].portrait_id = CHAR_ARIA;
    chars[GC_ARIA].text_color = TEXT_COLOR_CYAN;
    chars[GC_ARIA].traits = TRAIT_CURIOUS;
    chars[GC_ARIA].preferred_gift = GIFT_DATA;
    chars[GC_ARIA].secondary_gift = GIFT_BOOK;
    chars[GC_ARIA].disliked_gift = GIFT_WEAPON;
    chars[GC_ARIA].gift_liked_msg = "Fascinating data. I am... processing gratitude.";
    chars[GC_ARIA].gift_neutral_msg = "I have catalogued this item. Thank you, Kira.";
    chars[GC_ARIA].gift_disliked_msg = "I cannot comprehend the purpose of this object.";
    /* ARIA's heart event thresholds */
    chars[GC_ARIA].heart_thresholds[0].chapter = 0;
    chars[GC_ARIA].heart_thresholds[0].affinity_req = AFFINITY_HEART1;
    chars[GC_ARIA].heart_thresholds[1].chapter = 1;
    chars[GC_ARIA].heart_thresholds[1].affinity_req = AFFINITY_HEART2;
    chars[GC_ARIA].heart_thresholds[2].chapter = 2;
    chars[GC_ARIA].heart_thresholds[2].affinity_req = AFFINITY_HEART3;
    chars[GC_ARIA].heart_thresholds[3].chapter = 3;
    chars[GC_ARIA].heart_thresholds[3].affinity_req = AFFINITY_HEART4;
    chars[GC_ARIA].heart_thresholds[4].chapter = 4;
    chars[GC_ARIA].heart_thresholds[4].affinity_req = AFFINITY_CONFESSION;

    /* Dr. Vex - The Scientist */
    chars[GC_VEX].name = "Dr. Vex";
    chars[GC_VEX].full_name = "Dr. Elara Vex";
    chars[GC_VEX].faction = FACTION_NEUTRAL_SCIENTISTS;
    chars[GC_VEX].affinity = 20;
    chars[GC_VEX].portrait_id = CHAR_VEX;
    chars[GC_VEX].text_color = TEXT_COLOR_ORANGE;
    chars[GC_VEX].traits = TRAIT_GENTLE | TRAIT_SECRETIVE;
    chars[GC_VEX].preferred_gift = GIFT_BOOK;
    chars[GC_VEX].secondary_gift = GIFT_FLOWER;
    chars[GC_VEX].disliked_gift = GIFT_WEAPON;
    chars[GC_VEX].gift_liked_msg = "Oh... you remembered I love reading. Thank you, Kira.";
    chars[GC_VEX].gift_neutral_msg = "How kind. I wasn't expecting anything.";
    chars[GC_VEX].gift_disliked_msg = "I've caused enough destruction already...";
    /* Vex's heart event thresholds */
    chars[GC_VEX].heart_thresholds[0].chapter = 0;
    chars[GC_VEX].heart_thresholds[0].affinity_req = AFFINITY_HEART1;
    chars[GC_VEX].heart_thresholds[1].chapter = 1;
    chars[GC_VEX].heart_thresholds[1].affinity_req = AFFINITY_HEART2;
    chars[GC_VEX].heart_thresholds[2].chapter = 2;
    chars[GC_VEX].heart_thresholds[2].affinity_req = AFFINITY_HEART3;
    chars[GC_VEX].heart_thresholds[3].chapter = 3;
    chars[GC_VEX].heart_thresholds[3].affinity_req = AFFINITY_HEART4;
    chars[GC_VEX].heart_thresholds[4].chapter = 4;
    chars[GC_VEX].heart_thresholds[4].affinity_req = AFFINITY_CONFESSION;

    /* Init affinities from character defaults */
    {
        int i;
        for (i = 0; i < GC_COUNT; i++) {
            affinities[i] = chars[i].affinity;
        }
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

s8 game_chars_gift_preference(int char_id, int gift_type) {
    if (char_id < 1 || char_id >= GC_COUNT) return 0;
    const GameCharacter *ch = &chars[char_id];

    if (gift_type == ch->preferred_gift) return GIFT_PREFERRED_AFFINITY;
    if (gift_type == ch->secondary_gift) return GIFT_NEUTRAL_AFFINITY + 5; /* +10 for secondary */
    if (gift_type == ch->disliked_gift) return GIFT_DISLIKED_AFFINITY;
    return GIFT_NEUTRAL_AFFINITY;
}

const char* game_chars_gift_reaction_msg(int char_id, int gift_type) {
    if (char_id < 1 || char_id >= GC_COUNT) return "";
    const GameCharacter *ch = &chars[char_id];

    if (gift_type == ch->preferred_gift) return ch->gift_liked_msg;
    if (gift_type == ch->disliked_gift) return ch->gift_disliked_msg;
    return ch->gift_neutral_msg;
}

int game_chars_get_li_index(int route_id) {
    if (route_id >= ROUTE_NOVA && route_id <= ROUTE_VEX) {
        return route_id; /* Routes 1-4 map to character IDs 1-4 */
    }
    return -1;
}