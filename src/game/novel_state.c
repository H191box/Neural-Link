#include "novel_state.h"
#include "system.h"
#include "input.h"
#include "script_engine.h"
#include "script_data.h"
#include "dialog_box.h"
#include "transition.h"
#include "title_screen.h"
#include "chapter_title.h"
#include "hacking_minigame.h"
#include "gallery.h"
#include "settings.h"
#include "save_load.h"
#include "endings.h"
#include "bg_manager.h"
#include "text_renderer.h"
#include "flags.h"
#include "faction.h"
#include "characters_game.h"
#include "audio.h"
#include "gift_system.h"
#include "heart_events.h"
#include "route_system.h"

static GameState current_state = STATE_TITLE;
static char chapter_title_text[40];
static int chapter_intro_timer = 0;
static int current_script_id = 0;
static int choice_labels[4]; /* Label mapping for choices */
static int wait_counter = 0;

void novel_state_init(void) {
    current_state = STATE_TITLE;
    chapter_title_text[0] = '\0';
    chapter_intro_timer = 0;
    current_script_id = 0;
    wait_counter = 0;
}

void novel_state_set_state(GameState state) {
    current_state = state;

    switch (state) {
        case STATE_CHAPTER_INTRO:
            chapter_intro_timer = 0;
            break;
        case STATE_DIALOG:
            break;
        case STATE_HACKING:
            break;
        case STATE_GIFT_SELECT:
            gift_ui_init();
            break;
        case STATE_HEART_EVENT:
            heart_events_ui_init();
            break;
        case STATE_ROUTE_LOCK:
            route_lock_ui_init();
            break;
        default:
            break;
    }
}

GameState novel_state_get_state(void) {
    return current_state;
}

void novel_state_set_chapter_title(const char *title) {
    int i;
    for (i = 0; i < 39 && title[i] != '\0'; i++) {
        chapter_title_text[i] = title[i];
    }
    chapter_title_text[i] = '\0';
}

const char* novel_state_get_chapter_title(void) {
    return chapter_title_text;
}

void novel_state_new_game(void) {
    /* Reset all game systems */
    flags_clear();
    faction_reset();
    game_chars_init();
    gift_init();
    heart_events_init();
    route_init();

    /* Load Chapter 1 */
    current_script_id = SCRIPT_CH1;
    int count = 0;
    const char **lines = script_get(SCRIPT_CH1, &count);
    script_load(lines, count);

    /* Transition to dialog */
    novel_state_set_state(STATE_DIALOG);
}

void novel_state_continue_game(int slot) {
    /* Load save data */
    save_read(slot);

    /* Reload current script at saved position */
    const ScriptState *ss = script_get_state();
    int count = 0;
    const char **lines = script_get(ss->chapter, &count);
    script_load(lines, count);

    novel_state_set_state(STATE_DIALOG);
}

void novel_state_update(void) {
    switch (current_state) {
        case STATE_TITLE:
            title_screen_update();
            break;

        case STATE_CHAPTER_INTRO:
            chapter_intro_timer++;
            if (chapter_intro_timer > 180) { /* 3 seconds */
                novel_state_set_state(STATE_DIALOG);
            }
            if (input_pressed(KEY_A) && chapter_intro_timer > 30) {
                novel_state_set_state(STATE_DIALOG);
            }
            break;

        case STATE_DIALOG: {
            /* Handle transitions */
            if (trans_active()) {
                trans_update();
                script_handle_pending();
                break;
            }

            /* Update dialog box */
            dialog_update();

            /* Check if dialog is waiting for input and player pressed A */
            if (dialog_visible() && dialog_choice_made()) {
                if (dialog_get_choice() >= 0) {
                    /* Choice was made */
                    script_make_choice(dialog_get_choice());
                    dialog_close();
                    dialog_clear_choices();
                } else {
                    /* Text was dismissed, advance */
                    dialog_close();
                    script_make_choice(-1);
                }
                script_run_line();
                break;
            }

            /* If script is waiting, don't advance */
            if (script_is_waiting()) {
                /* Check if waiting for transition to complete */
                if (!trans_active() && trans_done()) {
                    trans_init(); /* Reset transition done flag */
                    script_make_choice(-1);
                    /* Don't advance if still in a wait/trans state */
                    if (novel_state_get_state() == STATE_CHAPTER_INTRO ||
                        novel_state_get_state() == STATE_HACKING) {
                        break;
                    }
                }
                break;
            }

            /* Run next script line */
            script_run_line();

            /* Check if we transitioned to another state */
            if (current_state != STATE_DIALOG) break;

            /* Update transition */
            if (trans_active()) {
                trans_update();
            }
            break;
        }

        case STATE_CHOICE:
            dialog_update();
            if (dialog_choice_made()) {
                int choice = dialog_get_choice();
                dialog_clear_choices();
                dialog_close();
                script_make_choice(choice);
                novel_state_set_state(STATE_DIALOG);
                script_run_line();
            }
            break;

        case STATE_HACKING: {
            hack_update();
            if (hack_is_complete()) {
                if (hack_success()) {
                    /* Sync succeeded — romantic/intimate scene continues */
                    sfx_play(SFX_HACK_SUCCESS);
                    flag_set("neural_sync_success", TRUE);
                    dialog_open();
                    dialog_set_text("Hearts synchronized. A wave of warmth floods through the link.");
                } else {
                    /* Sync failed — connection lost */
                    sfx_play(SFX_HACK_FAIL);
                    flag_set("neural_sync_failed", TRUE);
                    dialog_open();
                    dialog_set_text("Frequency mismatch. The connection fades like a half-remembered dream.");
                }
                novel_state_set_state(STATE_DIALOG);
                script_run_line();
            }
            break;
        }

        case STATE_GIFT_SELECT: {
            gift_ui_update();
            if (gift_ui_is_done()) {
                int result = gift_ui_get_result();
                if (result >= 0) {
                    /* Gift was given — apply affinity change */
                    dialog_open();
                    dialog_set_text(gift_ui_get_reaction_msg());
                }
                novel_state_set_state(STATE_DIALOG);
                script_run_line();
            }
            if (input_pressed(KEY_B)) {
                /* Cancel gift selection */
                novel_state_set_state(STATE_DIALOG);
                script_run_line();
            }
            break;
        }

        case STATE_HEART_EVENT: {
            heart_events_ui_update();
            if (heart_events_ui_is_done()) {
                novel_state_set_state(STATE_DIALOG);
                script_run_line();
            }
            break;
        }

        case STATE_ROUTE_LOCK: {
            route_lock_ui_update();
            if (route_lock_ui_is_done()) {
                int route = route_lock_ui_get_result();
                if (route >= ROUTE_NOVA && route <= ROUTE_VEX) {
                    route_lock_in(route);
                    /* Load route-specific chapter 4 */
                    int ch4_scripts[] = {
                        SCRIPT_CH4_NOVA, SCRIPT_CH4_CHEN,
                        SCRIPT_CH4_ARIA, SCRIPT_CH4_VEX
                    };
                    int count = 0;
                    const char **lines = script_get(ch4_scripts[route - 1], &count);
                    script_load(lines, count);
                }
                novel_state_set_state(STATE_DIALOG);
            }
            if (input_pressed(KEY_B)) {
                /* Auto-lock to highest affinity */
                route_auto_lock();
                novel_state_set_state(STATE_DIALOG);
            }
            break;
        }

        case STATE_CG_GALLERY:
            gallery_update();
            if (input_pressed(KEY_B)) {
                novel_state_set_state(STATE_TITLE);
                title_screen_init();
            }
            break;

        case STATE_SAVE_LOAD:
            /* Handled by save_load UI */
            if (input_pressed(KEY_B)) {
                novel_state_set_state(STATE_TITLE);
                title_screen_init();
            }
            break;

        case STATE_SETTINGS:
            settings_update();
            if (input_pressed(KEY_B)) {
                novel_state_set_state(STATE_TITLE);
                title_screen_init();
            }
            break;

        case STATE_ENDING:
            endings_update();
            if (input_pressed(KEY_START)) {
                novel_state_set_state(STATE_TITLE);
                title_screen_init();
            }
            break;
    }
}

void novel_state_render(void) {
    switch (current_state) {
        case STATE_TITLE:
            title_screen_render();
            break;

        case STATE_CHAPTER_INTRO:
            chapter_title_render(chapter_title_text);
            break;

        case STATE_DIALOG:
        case STATE_CHOICE:
            dialog_render();
            break;

        case STATE_HACKING:
            hack_render();
            break;

        case STATE_GIFT_SELECT:
            gift_ui_render();
            break;

        case STATE_HEART_EVENT:
            heart_events_ui_render();
            break;

        case STATE_ROUTE_LOCK:
            route_lock_ui_render();
            break;

        case STATE_CG_GALLERY:
            gallery_render();
            break;

        case STATE_SETTINGS:
            settings_render();
            break;

        case STATE_ENDING:
            endings_render();
            break;

        case STATE_SAVE_LOAD:
            /* Rendered by save_load module */
            break;
    }
}