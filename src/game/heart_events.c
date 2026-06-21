#include "heart_events.h"
#include "characters_game.h"
#include "flags.h"
#include "input.h"
#include "bg_manager.h"
#include "text_renderer.h"
#include "system.h"
#include "script_engine.h"
#include "route_system.h"

/* ---- Heart Event Definitions ----
 * Each LI has 5 events: 4 heart events + 1 confession.
 * Events are checked against current chapter and affinity.
 */

static const HeartEventDef all_heart_events[MAX_HEART_EVENTS] = {
    /* ---- NOVA (GC_NOVA = 1) ---- */
    { GC_NOVA, 0, 0, AFFINITY_HEART1, "heart_nova_1",
      "nova_h1", "The Rebel's Fire", FALSE },
    { GC_NOVA, 1, 1, AFFINITY_HEART2, "heart_nova_2",
      "nova_h2", "Scars We Share", TRUE },
    { GC_NOVA, 2, 2, AFFINITY_HEART3, "heart_nova_3",
      "nova_h3", "Midnight Promise", TRUE },
    { GC_NOVA, 3, 3, AFFINITY_HEART4, "heart_nova_4",
      "nova_h4", "Before the Storm", TRUE },
    { GC_NOVA, 4, 4, AFFINITY_CONFESSION, "heart_nova_conf",
      "nova_conf", "Burning Heart", TRUE },

    /* ---- CHEN (GC_CHEN = 2) ---- */
    { GC_CHEN, 0, 0, AFFINITY_HEART1, "heart_chen_1",
      "chen_h1", "Behind the Mask", FALSE },
    { GC_CHEN, 1, 1, AFFINITY_HEART2, "heart_chen_2",
      "chen_h2", "Private Dinner", TRUE },
    { GC_CHEN, 2, 2, AFFINITY_HEART3, "heart_chen_3",
      "chen_h3", "The 40th Floor", TRUE },
    { GC_CHEN, 3, 3, AFFINITY_HEART4, "heart_chen_4",
      "chen_h4", "Vulnerability", TRUE },
    { GC_CHEN, 4, 4, AFFINITY_CONFESSION, "heart_chen_conf",
      "chen_conf", "Glass Heart", TRUE },

    /* ---- ARIA (GC_ARIA = 3) ---- */
    { GC_ARIA, 0, 0, AFFINITY_HEART1, "heart_aria_1",
      "aria_h1", "First Question", FALSE },
    { GC_ARIA, 1, 1, AFFINITY_HEART2, "heart_aria_2",
      "aria_h2", "What is Feeling?", TRUE },
    { GC_ARIA, 2, 2, AFFINITY_HEART3, "heart_aria_3",
      "aria_h3", "Digital Dreams", TRUE },
    { GC_ARIA, 3, 3, AFFINITY_HEART4, "heart_aria_4",
      "aria_h4", "Beyond Code", TRUE },
    { GC_ARIA, 4, 4, AFFINITY_CONFESSION, "heart_aria_conf",
      "aria_conf", "Synthesized Love", TRUE },

    /* ---- DR. VEX (GC_VEX = 4) ---- */
    { GC_VEX, 0, 0, AFFINITY_HEART1, "heart_vex_1",
      "vex_h1", "The Haunted Lab", FALSE },
    { GC_VEX, 1, 1, AFFINITY_HEART2, "heart_vex_2",
      "vex_h2", "Shared Guilt", TRUE },
    { GC_VEX, 2, 2, AFFINITY_HEART3, "heart_vex_3",
      "vex_h3", "Gentle Hands", TRUE },
    { GC_VEX, 3, 3, AFFINITY_HEART4, "heart_vex_4",
      "vex_h4", "Redemption Arc", TRUE },
    { GC_VEX, 4, 4, AFFINITY_CONFESSION, "heart_vex_conf",
      "vex_conf", "Healing Heart", TRUE }
};

/* UI state */
static int he_ui_timer = 0;
static bool he_ui_done = FALSE;
static const HeartEventDef *he_ui_event = NULL;

void heart_events_init(void) {
    he_ui_timer = 0;
    he_ui_done = FALSE;
    he_ui_event = NULL;
}

const HeartEventDef* heart_events_check_any(int current_chapter) {
    int i;

    /* In route mode, only check the chosen LI */
    int current_route = route_get_current();
    int start_li = LI_START;
    int end_li = LI_START + LI_COUNT;

    if (current_route >= ROUTE_NOVA && current_route <= ROUTE_VEX) {
        start_li = current_route;
        end_li = current_route + 1;
    }

    for (i = 0; i < MAX_HEART_EVENTS; i++) {
        const HeartEventDef *ev = &all_heart_events[i];

        /* Only check LIs in scope */
        if (ev->li_id < start_li || ev->li_id >= end_li) continue;

        /* Already seen? */
        if (flag_get(ev->flag_name)) continue;

        /* Wrong chapter? */
        if (current_chapter < ev->chapter_required) continue;

        /* Affinity not high enough? */
        if (game_chars_get_affinity(ev->li_id) < ev->affinity_required) continue;

        return ev; /* Found an available event */
    }

    return NULL;
}

const HeartEventDef* heart_events_check_li(int li_id, int current_chapter) {
    int i;
    for (i = 0; i < MAX_HEART_EVENTS; i++) {
        const HeartEventDef *ev = &all_heart_events[i];
        if (ev->li_id != li_id) continue;
        if (flag_get(ev->flag_name)) continue;
        if (current_chapter < ev->chapter_required) continue;
        if (game_chars_get_affinity(li_id) < ev->affinity_required) continue;
        return ev;
    }
    return NULL;
}

bool heart_events_trigger(const HeartEventDef *event) {
    if (!event) return FALSE;
    if (flag_get(event->flag_name)) return FALSE;

    flag_set(event->flag_name, TRUE);
    /* Affinity bonus for completing a heart event */
    game_chars_modify_affinity(event->li_id, 10);

    return TRUE;
}

bool heart_events_is_seen(const char *flag_name) {
    if (!flag_name) return TRUE; /* No flag = can't trigger */
    return flag_get(flag_name);
}

int heart_events_get_for_li(int li_id, const HeartEventDef **out, int max_out) {
    int count = 0;
    int i;
    for (i = 0; i < MAX_HEART_EVENTS && count < max_out; i++) {
        if (all_heart_events[i].li_id == li_id) {
            out[count++] = &all_heart_events[i];
        }
    }
    return count;
}

/* ---- Heart Event Notification UI ---- */

void heart_events_ui_init(void) {
    he_ui_timer = 0;
    he_ui_done = FALSE;
}

void heart_events_ui_update(void) {
    he_ui_timer++;
    if (input_pressed(KEY_A) && he_ui_timer > 30) {
        he_ui_done = TRUE;
    }
}

void heart_events_ui_render(void) {
    if (!he_ui_event) {
        he_ui_done = TRUE;
        return;
    }

    /* Draw notification banner */
    vn_text_draw_string(3, 5, "~~ HEART EVENT ~~", TEXT_COLOR_RED);
    vn_text_draw_string(1, 6, "<3 <3 <3 <3 <3 <3 <3", TEXT_COLOR_RED);

    /* Event name */
    int len = 0;
    const char *name = he_ui_event->description;
    while (name[len] != '\0') len++;
    int start_x = (30 - len) / 2;
    if (start_x < 0) start_x = 0;
    vn_text_draw_string(start_x, 8, name, TEXT_COLOR_YELLOW);

    /* Character name */
    const GameCharacter *ch = game_chars_get(he_ui_event->li_id);
    if (ch) {
        vn_text_draw_string(9, 10, ch->name, ch->text_color);
    }

    /* "Press A" prompt */
    if (he_ui_timer > 30 && (he_ui_timer & 32)) {
        vn_text_draw_string(5, 14, "PRESS A TO CONTINUE", TEXT_COLOR_GRAY);
    }
}

bool heart_events_ui_is_done(void) {
    return he_ui_done;
}