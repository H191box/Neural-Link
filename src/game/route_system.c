#include "route_system.h"
#include "characters_game.h"
#include "flags.h"
#include "input.h"
#include "bg_manager.h"
#include "text_renderer.h"
#include "system.h"
#include "audio.h"

static int current_route = ROUTE_COMMON;
static bool locked = FALSE;

/* UI state */
static int rl_cursor = 0;
static bool rl_done = FALSE;
static int rl_result = -1;
static int rl_timer = 0;

void route_init(void) {
    current_route = ROUTE_COMMON;
    locked = FALSE;
    rl_cursor = 0;
    rl_done = FALSE;
    rl_result = -1;
    rl_timer = 0;
}

void route_lock_in(int route_id) {
    if (route_id >= ROUTE_NOVA && route_id <= ROUTE_VEX) {
        current_route = route_id;
        locked = TRUE;

        /* Set the route lock flag */
        const char *route_flags[] = {
            "", "route_locked_nova", "route_locked_chen",
            "route_locked_aria", "route_locked_vex"
        };
        flag_set(route_flags[route_id], TRUE);
    }
}

void route_auto_lock(void) {
    if (locked) return;

    int best_li = -1;
    int best_aff = 0;
    int i;

    for (i = LI_START; i < LI_START + LI_COUNT; i++) {
        u8 aff = game_chars_get_affinity(i);
        if (aff > best_aff) {
            best_aff = aff;
            best_li = i;
        }
    }

    if (best_li >= 0 && best_aff >= ROUTE_AUTO_LOCK_MIN) {
        route_lock_in(best_li);
    }
    /* If no LI meets threshold, stay in common route (default) */
}

int route_get_current(void) {
    return current_route;
}

bool route_is_locked(void) {
    return locked;
}

int route_get_ending_type(void) {
    if (!locked || current_route < ROUTE_NOVA || current_route > ROUTE_VEX) {
        return ENDING_BAD;
    }

    u8 affinity = game_chars_get_affinity(current_route);

    if (affinity >= 80) return ENDING_GOOD;
    if (affinity >= 40) return ENDING_NORMAL;
    return ENDING_BAD;
}

int route_get_current_li(void) {
    if (!locked) return -1;
    return current_route;
}

/* ---- Route Lock UI ---- */

static const char *route_names[] = {
    "", /* Index 0 unused (common) */
    "Nova",
    "Director Chen",
    "ARIA",
    "Dr. Vex"
};

static const char *route_descs[] = {
    "",
    "The rebel who fights for freedom",
    "The CEO who hides behind power",
    "The AI who yearns to feel",
    "The scientist seeking redemption"
};

void route_lock_ui_init(void) {
    rl_cursor = 0;
    rl_done = FALSE;
    rl_result = -1;
    rl_timer = 0;
}

void route_lock_ui_update(void) {
    rl_timer++;

    if (rl_done) return;

    if (input_pressed(KEY_UP)) {
        rl_cursor--;
        if (rl_cursor < 0) rl_cursor = LI_COUNT - 1;
        sfx_play(SFX_MENU_MOVE);
    }
    if (input_pressed(KEY_DOWN)) {
        rl_cursor++;
        if (rl_cursor >= LI_COUNT) rl_cursor = 0;
        sfx_play(SFX_MENU_MOVE);
    }

    if (input_pressed(KEY_A)) {
        rl_result = ROUTE_NOVA + rl_cursor;
        rl_done = TRUE;
        sfx_play(SFX_CHOICE_SELECT);
    }
}

void route_lock_ui_render(void) {
    int i;

    /* Title */
    vn_text_draw_string(4, 1, "WHO DO YOU TRUST?", TEXT_COLOR_RED);
    vn_text_draw_string(1, 2, "<3", TEXT_COLOR_RED);
    vn_text_draw_string(25, 2, "<3", TEXT_COLOR_RED);

    /* Decorative line */
    for (i = 3; i < 27; i++) {
        vn_text_draw_char(i, 3, '-', TEXT_COLOR_GRAY);
    }

    /* List each LI */
    int y = 5;
    for (i = 0; i < LI_COUNT; i++) {
        int li_id = ROUTE_NOVA + i;
        u16 name_color = TEXT_COLOR_WHITE;
        u8 affinity = game_chars_get_affinity(li_id);
        const GameCharacter *ch = game_chars_get(li_id);

        if (i == rl_cursor) {
            name_color = TEXT_COLOR_YELLOW;
            /* Cursor */
            if (system_get_frame() & 16) {
                vn_text_draw_char(2, y + i * 2, '>', TEXT_COLOR_YELLOW);
            }
        }

        /* LI name */
        vn_text_draw_string(4, y + i * 2, route_names[li_id], name_color);

        /* Description */
        vn_text_draw_string(4, y + i * 2 + 1, route_descs[li_id], TEXT_COLOR_GRAY);

        /* Affinity bar (visual: X marks per 20 affinity) */
        if (ch) {
            int bar_x = 22;
            int bar_y_pos = y + i * 2;
            int hearts = affinity / 20;
            int j;
            for (j = 0; j < 5; j++) {
                if (j < hearts) {
                    vn_text_draw_char(bar_x + j * 2, bar_y_pos, '<', ch->text_color);
                    vn_text_draw_char(bar_x + j * 2 + 1, bar_y_pos, '3', ch->text_color);
                } else {
                    vn_text_draw_char(bar_x + j * 2, bar_y_pos, '.', TEXT_COLOR_GRAY);
                    vn_text_draw_char(bar_x + j * 2 + 1, bar_y_pos, '.', TEXT_COLOR_GRAY);
                }
            }
        }
    }

    /* Instructions */
    vn_text_draw_string(3, 14, "A:Choose  B:Auto", TEXT_COLOR_GRAY);
    if (rl_timer > 60 && (rl_timer & 32)) {
        vn_text_draw_string(7, 15, "This choice is forever.", TEXT_COLOR_RED);
    }
}

bool route_lock_ui_is_done(void) {
    return rl_done;
}

int route_lock_ui_get_result(void) {
    return rl_result;
}