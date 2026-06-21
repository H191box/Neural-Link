#include "endings.h"
#include "input.h"
#include "bg_manager.h"
#include "text_renderer.h"
#include "system.h"
#include "flags.h"
#include "faction.h"

static int current_ending = -1;
static int ending_timer = 0;
static bool ending_text_complete = FALSE;

static const char *ending_names[] = {
    "ENDING A: REVOLUTION",
    "ENDING B: CORPORATE",
    "ENDING C: SACRIFICE",
    "ENDING D: MERGE",
    "ENDING E: HIDDEN TRUTH"
};

static const char *ending_texts[] = {
    /* Revolution */
    "The Resistance storms MegaCorp HQ. Neural implants are deactivated worldwide. A new era of freedom begins... but at what cost?",
    /* Corporate */
    "Kai ascends to Director of Neural Systems. The Network grows stronger. Humanity is connected, controlled, and at peace.",
    /* Sacrifice */
    "The neural network collapses. Technology regresses a century. But humanity is free to choose its own path once more.",
    /* Merge */
    "Human consciousness merges with ARIA. Kai becomes something more than human. A new form of life emerges from the digital sea.",
    /* Hidden Truth */
    "The truth behind the implants reveals they were never meant to control. They were meant to connect. ARIA was protecting humanity from itself all along."
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
    /* Determine ending based on flags, faction rep, and choices */
    u8 rep[3];
    faction_get_all(rep);
    
    /* Hidden Truth: specific combination */
    if (flag_get("was_honest_with_chen") && flag_get("agreed_to_help_resistance") && 
        flag_get("sought_third_path")) {
        return ENDING_HIDDEN_TRUTH;
    }
    
    /* Revolution: high resistance rep */
    if (rep[FACTION_RESISTANCE] > 60 && flag_get("joined_resistance")) {
        return ENDING_REVOLUTION;
    }
    
    /* Corporate: high megacorp rep */
    if (rep[FACTION_MEGACORP] > 60 && !flag_get("confronted_chen")) {
        return ENDING_CORPORATE;
    }
    
    /* Sacrifice: chose to destroy */
    if (flag_get("destroyed_system")) {
        return ENDING_SACRIFICE;
    }
    
    /* Default: Merge */
    return ENDING_MERGE;
}

void endings_update(void) {
    ending_timer++;
    
    if (input_pressed(KEY_A)) {
        ending_text_complete = TRUE;
    }
}

void endings_render(void) {
    if (current_ending < 0 || current_ending > 4) return;
    
    /* Fade background */
    bg_clear_map(BG_SCENE);
    
    /* Draw ending title */
    int len = 0;
    const char *name = ending_names[current_ending];
    while (name[len] != '\0') len++;
    int start_x = (30 - len) / 2;
    if (start_x < 0) start_x = 0;
    
    vn_text_draw_string(start_x, 4, name, TEXT_COLOR_CYAN);
    
    /* Decorative line */
    int i;
    for (i = 3; i < 27; i++) {
        vn_text_draw_char(i, 5, '-', TEXT_COLOR_GRAY);
    }
    
    /* Ending text */
    const char *text = ending_texts[current_ending];
    vn_text_draw_wrapped(2, 7, text, 26, 6, TEXT_COLOR_WHITE);
    
    /* "THE END" */
    if (ending_timer > 120) {
        int tl = 8;
        int tx = (30 - tl) / 2;
        vn_text_draw_string(tx, 15, "THE END", TEXT_COLOR_YELLOW);
    }
    
    /* Prompt */
    if (ending_timer > 180 && (ending_timer & 32)) {
        vn_text_draw_string(4, 18, "PRESS START TO CONTINUE", TEXT_COLOR_GRAY);
    }
}