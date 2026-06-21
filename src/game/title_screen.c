#include "title_screen.h"
#include "system.h"
#include "input.h"
#include "bg_manager.h"
#include "text_renderer.h"
#include "transition.h"
#include "novel_state.h"
#include "save_load.h"
#include "audio.h"
#include "sprite_manager.h"
#include "palettes.h"

static int menu_cursor = 0;
static int title_anim = 0;
static int glitch_timer = 0;
static int glitch_offset = 0;
static bool has_save_data = FALSE;

void title_screen_init(void) {
    menu_cursor = 0;
    title_anim = 0;
    glitch_timer = 0;
    glitch_offset = 0;
    has_save_data = save_has_data(0);

    /* Set up BG0 with dark background */
    bg_clear_map(BG_SCENE);
    bg_clear_map(BG_CHAR);
    bg_clear_map(BG_UI);
    vn_text_clear_all();

    /* Disable BG1 (characters) and BG3 (dialog) on title */
    bg_enable(BG_CHAR, FALSE);
    bg_enable(BG_DIALOG, FALSE);
    bg_enable(BG_UI, TRUE);
    bg_enable(BG_SCENE, TRUE);
}

void title_screen_update(void) {
    title_anim++;
    glitch_timer++;

    /* Random glitch effect — heartbeat pulse */
    if (glitch_timer > 60 + (title_anim % 120)) {
        glitch_timer = 0;
        glitch_offset = (title_anim * 7) % 5;
    }

    if (input_pressed(KEY_UP)) {
        menu_cursor--;
        if (menu_cursor < 0) menu_cursor = MENU_COUNT - 1;
        sfx_play(SFX_MENU_MOVE);
    }
    if (input_pressed(KEY_DOWN)) {
        menu_cursor++;
        if (menu_cursor >= MENU_COUNT) menu_cursor = 0;
        sfx_play(SFX_MENU_MOVE);
    }

    if (input_pressed(KEY_A)) {
        sfx_play(SFX_CHOICE_SELECT);
        switch (menu_cursor) {
            case MENU_NEW_GAME:
                bg_enable(BG_CHAR, TRUE);
                bg_enable(BG_DIALOG, TRUE);
                trans_start(TRANS_FADE_OUT, 20);
                novel_state_new_game();
                break;
            case MENU_CONTINUE:
                if (has_save_data) {
                    bg_enable(BG_CHAR, TRUE);
                    bg_enable(BG_DIALOG, TRUE);
                    novel_state_continue_game(0);
                }
                break;
            case MENU_GALLERY:
                novel_state_set_state(STATE_CG_GALLERY);
                gallery_init();
                break;
            case MENU_SETTINGS:
                novel_state_set_state(STATE_SETTINGS);
                break;
        }
    }
}

void title_screen_render(void) {
    /* Draw title with glitch + heartbeat effect */
    int title_x = 7;
    int title_y = 2;

    /* Glitch: occasionally offset title text */
    int gx = 0, gy = 0;
    if (glitch_timer < 3 && glitch_timer > 0) {
        gx = glitch_offset - 2;
        gy = (glitch_offset % 2) - 1;
    }

    /* Title line 1: "NEURAL LINK" */
    vn_text_draw_string(title_x + gx, title_y + gy, "NEURAL", TEXT_COLOR_CYAN);
    vn_text_draw_string(title_x + gx + 2, title_y + 1 + gy, "LINK", TEXT_COLOR_CYAN);

    /* Subtitle: "HEARTSYNC" in red/pink */
    vn_text_draw_string(title_x + 1, title_y + 3, "HEARTSYNC", TEXT_COLOR_RED);

    /* Heartbeat pulse — alternating brightness */
    if (title_anim & 32) {
        vn_text_draw_string(6, title_y + 2, "<3", TEXT_COLOR_RED);
    }

    /* Draw glitch lines (random horizontal corruption) */
    if (glitch_timer < 2 && glitch_timer > 0) {
        int i;
        for (i = 0; i < 30; i++) {
            vn_text_draw_char(i, title_y + glitch_offset, (title_anim + i) & 0x7E, TEXT_COLOR_RED);
        }
    }

    /* Tagline */
    vn_text_draw_string(1, 6, "The deepest connection", TEXT_COLOR_GRAY);
    vn_text_draw_string(3, 7, "is between hearts.", TEXT_COLOR_GRAY);

    /* Version text */
    vn_text_draw_string(1, 19, NL_VERSION_STRING, TEXT_COLOR_GRAY);

    /* Draw menu */
    int menu_y = 9;
    const char *menu_items[] = {"NEW GAME", "CONTINUE", "GALLERY", "SETTINGS"};
    int i;
    for (i = 0; i < MENU_COUNT; i++) {
        u16 color = TEXT_COLOR_GRAY;
        if (i == menu_cursor) {
            color = TEXT_COLOR_YELLOW;
            /* Blinking heart cursor instead of arrow */
            if (title_anim & 16) {
                vn_text_draw_char(8, menu_y + i, '<', TEXT_COLOR_RED);
                vn_text_draw_char(9, menu_y + i, '3', TEXT_COLOR_RED);
            }
        }

        /* Gray out "Continue" if no save data */
        if (i == MENU_CONTINUE && !has_save_data) {
            color = TEXT_COLOR_GRAY;
        }

        vn_text_draw_string(11, menu_y + i, menu_items[i], color);
    }

    /* "Press A to select" blinking text at bottom */
    if (title_anim & 32) {
        vn_text_draw_string(7, 18, "PRESS A TO SELECT", TEXT_COLOR_WHITE);
    }
}