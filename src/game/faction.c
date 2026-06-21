#include "faction.h"

static u8 reputations[FACTION_COUNT];

void faction_init(void) {
    reputations[FACTION_RESISTANCE] = 30;
    reputations[FACTION_MEGACORP] = 50;
    reputations[FACTION_NEUTRALS] = 50;
    reputations[FACTION_NEUTRAL_SCIENTISTS] = 25;
    reputations[FACTION_AI_COLLECTIVE] = 40;
}

void faction_modify(int faction_id, s16 delta) {
    if (faction_id < 0 || faction_id >= FACTION_COUNT) return;
    s16 val = (s16)reputations[faction_id] + delta;
    reputations[faction_id] = (u8)CLAMP(val, 0, 100);
}

u8 faction_get(int faction_id) {
    if (faction_id < 0 || faction_id >= FACTION_COUNT) return 0;
    return reputations[faction_id];
}

void faction_set(int faction_id, u8 value) {
    if (faction_id < 0 || faction_id >= FACTION_COUNT) return;
    reputations[faction_id] = CLAMP(value, 0, 100);
}

int faction_get_highest(void) {
    int best = 0;
    int best_val = reputations[0];
    int i;
    for (i = 1; i < FACTION_COUNT; i++) {
        if (reputations[i] > best_val) {
            best_val = reputations[i];
            best = i;
        }
    }
    return best;
}

void faction_get_all(u8 rep[FACTION_COUNT]) {
    int i;
    for (i = 0; i < FACTION_COUNT; i++) {
        rep[i] = reputations[i];
    }
}

void faction_reset(void) {
    faction_init();
}