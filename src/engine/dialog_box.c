#include "dialog_box.h"
#include "bg_manager.h"
#include "gba_header.h"
#include "input.h"
#include "text_renderer.h"

static DialogBox dlg;

/* UI tile indices (loaded from ui_elements.c) */
extern const u16 ui_tiles[32]; /* Frame tiles: corner TL, TR, BL, BR, edge T, B, L, R, fill */
extern const u16 ui_palette[16];

#define UI_TILE_FILL   0
#define UI_TILE_TOP    1
#define UI_TILE_BOTTOM 2
#define UI_TILE_LEFT   3
#define UI_TILE_RIGHT  4
#define UI_TILE_TL     5
#define UI_TILE_TR     6
#define UI_TILE_BL     7
#define UI_TILE_BR     8
#define UI_TILE_ARROW  9

void dialog_init(void) {
    dlg.visible = FALSE;
    dlg.speaker[0] = '\0';
    dlg.text[0] = '\0';
    dlg.num_choices = 0;
    dlg.selected_choice = 0;
    dlg.waiting_for_input = FALSE;
    dlg.showing_choices = FALSE;
    dlg.choice_made = FALSE;
    dlg.choice_blink_timer = 0;
    my_memset(&dlg.tw, 0, sizeof(TypewriterState));
}

void dialog_open(void) {
    dlg.visible = TRUE;
    dlg.waiting_for_input = FALSE;
    dlg.showing_choices = FALSE;
    dlg.choice_made = FALSE;
    dlg.selected_choice = 0;
    dlg.num_choices = 0;
    dlg.choice_blink_timer = 0;
}

void dialog_close(void) {
    dlg.visible = FALSE;
    dlg.speaker[0] = '\0';
    dlg.text[0] = '\0';
    dlg.num_choices = 0;
    dlg.waiting_for_input = FALSE;
    dlg.showing_choices = FALSE;
    dlg.choice_made = FALSE;
    vn_text_clear_area();
}

void dialog_set_speaker(const char *name) {
    int i;
    for (i = 0; i < 19 && name[i] != '\0'; i++) {
        dlg.speaker[i] = name[i];
    }
    dlg.speaker[i] = '\0';
}

void dialog_set_text(const char *text) {
    int i;
    for (i = 0; i < 255 && text[i] != '\0'; i++) {
        dlg.text[i] = text[i];
    }
    dlg.text[i] = '\0';
    
    dlg.waiting_for_input = FALSE;
    dlg.choice_made = FALSE;
    
    /* Clear text area and start typewriter */
    vn_text_clear_area();
    vn_typewriter_start(&dlg.tw, text, DBOX_TEXT_X, DBOX_TEXT_Y + 1,
                        DBOX_TEXT_W, DBOX_TEXT_H - 1, TEXT_COLOR_WHITE);
}

void dialog_add_choice(const char *text, int label_id) {
    if (dlg.num_choices >= MAX_CHOICES) return;
    
    int i;
    for (i = 0; i < 39 && text[i] != '\0'; i++) {
        dlg.choices[dlg.num_choices].text[i] = text[i];
    }
    dlg.choices[dlg.num_choices].text[i] = '\0';
    dlg.choices[dlg.num_choices].label_id = label_id;
    dlg.num_choices++;
}

void dialog_clear_choices(void) {
    dlg.num_choices = 0;
    dlg.selected_choice = 0;
    dlg.showing_choices = FALSE;
    dlg.choice_made = FALSE;
}

void dialog_update(void) {
    if (!dlg.visible) return;
    
    dlg.choice_blink_timer++;
    
    /* Update typewriter */
    if (!vn_typewriter_is_complete(&dlg.tw)) {
        vn_typewriter_update(&dlg.tw);
        
        /* A button to skip to end */
        if (input_pressed(KEY_A)) {
            vn_typewriter_skip(&dlg.tw);
        }
    } else {
        dlg.waiting_for_input = TRUE;
        
        if (!dlg.showing_choices) {
            /* Wait for A press to advance or show choices */
            if (input_pressed(KEY_A)) {
                if (dlg.num_choices > 0) {
                    dlg.showing_choices = TRUE;
                } else {
                    /* No choices, signal text complete */
                    dlg.choice_made = TRUE; /* Reuse as "done" flag */
                }
            }
        } else {
            /* Handle choice selection */
            if (input_pressed(KEY_UP)) {
                dlg.selected_choice--;
                if (dlg.selected_choice < 0) dlg.selected_choice = dlg.num_choices - 1;
            }
            if (input_pressed(KEY_DOWN)) {
                dlg.selected_choice++;
                if (dlg.selected_choice >= dlg.num_choices) dlg.selected_choice = 0;
            }
            if (input_pressed(KEY_A)) {
                dlg.choice_made = TRUE;
            }
        }
    }
}

void dialog_render(void) {
    if (!dlg.visible) return;
    
    /* Draw dialog box frame on BG2 (UI layer) using tile map */
    /* We draw a simple rectangle with border */
    int r, c;
    
    /* Draw filled rectangle for dialog background */
    for (r = DBOX_Y; r < DBOX_Y + DBOX_H; r++) {
        for (c = DBOX_X; c < DBOX_X + DBOX_W; c++) {
            u16 tile = UI_TILE_FILL;
            /* Add palette */
            if (r == DBOX_Y && c == DBOX_X) tile = UI_TILE_TL;
            else if (r == DBOX_Y && c == DBOX_X + DBOX_W - 1) tile = UI_TILE_TR;
            else if (r == DBOX_Y + DBOX_H - 1 && c == DBOX_X) tile = UI_TILE_BL;
            else if (r == DBOX_Y + DBOX_H - 1 && c == DBOX_X + DBOX_W - 1) tile = UI_TILE_BR;
            else if (r == DBOX_Y) tile = UI_TILE_TOP;
            else if (r == DBOX_Y + DBOX_H - 1) tile = UI_TILE_BOTTOM;
            else if (c == DBOX_X) tile = UI_TILE_LEFT;
            else if (c == DBOX_X + DBOX_W - 1) tile = UI_TILE_RIGHT;
            
            bg_set_tile_pal(BG_UI, c, r, tile, 1);
        }
    }
    
    /* Draw speaker name if set */
    if (dlg.speaker[0] != '\0') {
        vn_text_draw_string(DBOX_TEXT_X, DBOX_TEXT_Y, dlg.speaker, TEXT_COLOR_CYAN);
    }
    
    /* Draw choices if showing */
    if (dlg.showing_choices) {
        int i;
        int choice_y = DBOX_Y + DBOX_H - dlg.num_choices - 1;
        for (i = 0; i < dlg.num_choices; i++) {
            u16 color = (i == dlg.selected_choice) ? TEXT_COLOR_YELLOW : TEXT_COLOR_GRAY;
            /* Draw selection arrow */
            if (i == dlg.selected_choice && (dlg.choice_blink_timer & 16)) {
                vn_text_draw_char(DBOX_TEXT_X, choice_y + i, '>', TEXT_COLOR_YELLOW);
            }
            vn_text_draw_string(DBOX_TEXT_X + 2, choice_y + i, dlg.choices[i].text, color);
        }
    }
    
    /* Draw "wait for input" indicator (blinking triangle) */
    if (dlg.waiting_for_input && !dlg.showing_choices && (dlg.choice_blink_timer & 16)) {
        vn_text_draw_char(DBOX_X + DBOX_W - 3, DBOX_Y + DBOX_H - 2, 0x10, TEXT_COLOR_WHITE);
    }
}

bool dialog_text_complete(void) {
    return vn_typewriter_is_complete(&dlg.tw);
}

bool dialog_choice_made(void) {
    return dlg.choice_made;
}

int dialog_get_choice(void) {
    if (!dlg.choice_made || dlg.num_choices == 0) return -1;
    return dlg.selected_choice;
}

bool dialog_visible(void) {
    return dlg.visible;
}

void dialog_clear(void) {
    vn_text_clear_area();
}