#include "gallery.h"
#include "input.h"
#include "bg_manager.h"
#include "text_renderer.h"
#include "system.h"
#include "audio.h"

static bool cg_unlocked[MAX_CGS];
static int gallery_cursor = 0;
static int gallery_page = 0;
static int gallery_anim = 0;

/* CG names for display */
static const char *cg_names[MAX_CGS] = {
    "City Night View",
    "Neural Lab",
    "Kai Portrait",
    "Nova Portrait",
    "Director Chen",
    "ARIA Interface",
    "Dr. Vex Lab",
    "Resistance Base",
    "Neural Network",
    "The Signal",
    "Confrontation",
    "The Hack",
    "City Under Rain",
    "Phase 3 Launch",
    "Revolution",
    "Convergence"
};

void gallery_init(void) {
    int i;
    for (i = 0; i < MAX_CGS; i++) {
        cg_unlocked[i] = flag_get_index(i) || i < 2; /* First 2 always unlocked */
    }
    gallery_cursor = 0;
    gallery_page = 0;
    gallery_anim = 0;
    
    bg_clear_map(BG_UI);
    vn_text_clear_all();
    bg_enable(BG_DIALOG, FALSE);
}

void gallery_unlock(int cg_id) {
    if (cg_id >= 0 && cg_id < MAX_CGS) {
        cg_unlocked[cg_id] = TRUE;
    }
}

bool gallery_is_unlocked(int cg_id) {
    if (cg_id >= 0 && cg_id < MAX_CGS) return cg_unlocked[cg_id];
    return FALSE;
}

void gallery_update(void) {
    gallery_anim++;
    
    int items_per_page = 8;
    
    if (input_pressed(KEY_LEFT)) {
        gallery_cursor--;
        if (gallery_cursor < gallery_page * items_per_page) {
            if (gallery_page > 0) {
                gallery_page--;
                gallery_cursor = gallery_page * items_per_page + items_per_page - 1;
            } else {
                gallery_cursor = 0;
            }
        }
        sfx_play(SFX_MENU_MOVE);
    }
    if (input_pressed(KEY_RIGHT)) {
        gallery_cursor++;
        if (gallery_cursor >= (gallery_page + 1) * items_per_page || gallery_cursor >= MAX_CGS) {
            if (gallery_cursor < MAX_CGS) {
                gallery_page++;
            } else {
                gallery_cursor = MAX_CGS - 1;
            }
        }
        sfx_play(SFX_MENU_MOVE);
    }
    if (input_pressed(KEY_UP)) {
        gallery_cursor -= 4;
        if (gallery_cursor < 0) gallery_cursor = 0;
        gallery_page = (gallery_cursor / items_per_page) * 1;
        sfx_play(SFX_MENU_MOVE);
    }
    if (input_pressed(KEY_DOWN)) {
        gallery_cursor += 4;
        if (gallery_cursor >= MAX_CGS) gallery_cursor = MAX_CGS - 1;
        sfx_play(SFX_MENU_MOVE);
    }
}

void gallery_render(void) {
    /* Title */
    vn_text_draw_string(9, 0, "CG GALLERY", TEXT_COLOR_CYAN);
    
    /* Draw grid of CG slots (4 per row, 2 rows per page) */
    int items_per_page = 8;
    int start = gallery_page * items_per_page;
    int i;
    
    for (i = 0; i < items_per_page && (start + i) < MAX_CGS; i++) {
        int idx = start + i;
        int row = i / 4;
        int col = i % 4;
        int sx = 2 + col * 7;
        int sy = 3 + row * 5;
        
        u16 color = (idx == gallery_cursor) ? TEXT_COLOR_YELLOW : TEXT_COLOR_GRAY;
        
        /* Draw frame */
        int fx, fy;
        for (fx = 0; fx < 6; fx++) {
            vn_text_draw_char(sx + fx, sy, '-', color);
            vn_text_draw_char(sx + fx, sy + 3, '-', color);
        }
        vn_text_draw_char(sx, sy + 1, '|', color);
        vn_text_draw_char(sx + 5, sy + 1, '|', color);
        vn_text_draw_char(sx, sy + 2, '|', color);
        vn_text_draw_char(sx + 5, sy + 2, '|', color);
        
        if (cg_unlocked[idx]) {
            /* Draw CG name */
            vn_text_draw_string(sx, sy + 1, cg_names[idx], TEXT_COLOR_WHITE);
        } else {
            /* Locked */
            vn_text_draw_string(sx + 1, sy + 1, "LOCKED", TEXT_COLOR_RED);
        }
    }
    
    /* Page indicator */
    vn_text_draw_string(12, 13, (gallery_anim & 16) ? "L/R:Page  B:Back" : "", TEXT_COLOR_GRAY);
}