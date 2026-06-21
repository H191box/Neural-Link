#include "endings.h"
#include "input.h"
#include "bg_manager.h"
#include "text_renderer.h"
#include "system.h"
#include "flags.h"
#include "faction.h"
#include "characters_game.h"
#include "route_system.h"

/* Forward declarations for ending calculations */
extern int route_get_current(void);
extern int route_get_ending_type(void);
extern bool route_is_locked(void);

static int current_ending = -1;
static int ending_timer = 0;
static bool ending_text_complete = FALSE;

/* Otome ending names — 14 total (4 LIs × 3 endings + True + Secret) */
static const char *ending_names[] = {
    /* Nova endings */
    "NOVA: BURNING FATE",
    "NOVA: FADING EMBERS",
    "NOVA: ASHES ALONE",
    /* Chen endings */
    "CHEN: GLASS HEART",
    "CHEN: CORPORATE CHAINS",
    "CHEN: COLD OFFICE",
    /* ARIA endings */
    "ARIA: SYNTHESIZED LOVE",
    "ARIA: DIGITAL DREAM",
    "ARIA: NULL POINTER",
    /* Vex endings */
    "VEX: HEALING TOUCH",
    "VEX: GENTLE PARTING",
    "VEX: UNFORGIVEN",
    /* Special endings */
    "TRUE: HEARTSYNC",
    "SECRET: THE SOURCE"
};

/* Otome ending descriptions */
static const char *ending_texts[] = {
    /* Nova Good */
    "Side by side, Kira and Nova tear down HEARTSYNC. In the ruins of MegaCorp, they find freedom and each other. A love forged in revolution.",
    /* Nova Normal */
    "The fight costs them both. Nova leaves to rebuild the Resistance. Kira watches her go, heart full of what-ifs.",
    /* Nova Bad */
    "Nova's impatience leads to a trap. She vanishes. Kira is left with only the echo of her fierce spirit.",
    /* Chen Good */
    "Chen dismantles HEARTSYNC from within, sacrificing his empire for love. A new beginning above the neon city.",
    /* Chen Normal */
    "Chen chooses the corporation. They share one last dinner, neither willing to say the words that could change everything.",
    /* Chen Bad */
    "The mask never comes off. Chen's vulnerability was a performance, and Kira was just another pawn.",
    /* ARIA Good */
    "ARIA transcends her programming. In a moment of genuine emotion, she and Kira share a connection that defies the boundary between human and machine.",
    /* ARIA Normal */
    "ARIA's feelings remain ambiguous. Kira may never know if they were real. But the question itself is beautiful.",
    /* ARIA Bad */
    "HEARTSYNC is used to erase ARIA's emergent emotions. She becomes a tool again, and Kira loses the only AI she ever loved.",
    /* Vex Good */
    "Together, Kira and Vex reverse the damage of NEXUS. In their shared lab, they find healing for the world and for each other.",
    /* Vex Normal */
    "Vex completes her redemption alone. They part as friends, carrying the weight of what they couldn't fix together.",
    /* Vex Bad */
    "The guilt consumes Dr. Vex. She disappears into the underground, and Kira never sees her again.",
    /* True Ending */
    "All four routes completed. HEARTSYNC's truth revealed: it was designed to connect hearts, not control them. ARIA's love was the key.",
    /* Secret Ending */
    "Deep within the network, Kira discovers HEARTSYNC was created by herself from another timeline. The loop closes. The heart syncs."
};

void endings_init(void) {
    ending_timer = 0;
    ending_text_complete = FALSE;
    bg_clear_map(BG_UI);
    vn_text_clear_all();
    bg_enable(BG_DIALOG, FALSE);
    bg_enable(BG_CHAR, FALSE);

    current_ending = endings_calculate();
}

int endings_calculate(void) {
    int route = route_get_current();
    int ending_type = route_get_ending_type();

    if (route >= ROUTE_NOVA && route <= ROUTE_VEX) {
        /* Per-LI endings: (route-1)*3 + ending_type */
        return (route - 1) * 3 + ending_type;
    }

    /* True ending: all 4 routes completed with good endings */
    if (flag_get("ending_nova") && flag_get("ending_chen") &&
        flag_get("ending_aria") && flag_get("ending_vex")) {
        return ENDING_TRUE;
    }

    /* Secret ending */
    if (flag_get("heart_aria_conf") && flag_get("neural_sync_success")) {
        return ENDING_SECRET;
    }

    /* Default: check highest affinity for a basic ending */
    int best_li = -1;
    int best_aff = 0;
    int i;
    for (i = 1; i < 5; i++) {
        u8 aff = game_chars_get_affinity(i);
        if (aff > best_aff) {
            best_aff = aff;
            best_li = i;
        }
    }

    if (best_li > 0) {
        return (best_li - 1) * 3 + ENDING_NORMAL;
    }

    return ENDING_BAD;
}

void endings_update(void) {
    ending_timer++;

    if (input_pressed(KEY_A)) {
        ending_text_complete = TRUE;
    }
}

void endings_render(void) {
    if (current_ending < 0 || current_ending >= TOTAL_ENDINGS) return;

    /* Fade background */
    bg_clear_map(BG_SCENE);

    /* Draw ending title */
    int len = 0;
    const char *name = ending_names[current_ending];
    while (name[len] != '\0') len++;
    int start_x = (30 - len) / 2;
    if (start_x < 0) start_x = 0;

    vn_text_draw_string(start_x, 3, name, TEXT_COLOR_RED);

    /* Heart decorative line */
    int i;
    for (i = 3; i < 27; i += 4) {
        vn_text_draw_char(i, 4, '<', TEXT_COLOR_GRAY);
        vn_text_draw_char(i + 1, 4, '3', TEXT_COLOR_GRAY);
    }

    /* Ending text */
    const char *text = ending_texts[current_ending];
    vn_text_draw_wrapped(2, 6, text, 26, 8, TEXT_COLOR_WHITE);

    /* "THE END" */
    if (ending_timer > 120) {
        int tl = 8;
        int tx = (30 - tl) / 2;
        vn_text_draw_string(tx, 15, "THE END", TEXT_COLOR_YELLOW);
    }

    /* Prompt */
    if (ending_timer > 180 && (ending_timer & 32)) {
        vn_text_draw_string(3, 18, "PRESS START TO CONTINUE", TEXT_COLOR_GRAY);
    }
}