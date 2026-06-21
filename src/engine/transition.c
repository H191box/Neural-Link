#include "transition.h"
#include "gba_header.h"
#include "dma.h"

static Transition trans;

void trans_init(void) {
    trans.type = TRANS_NONE;
    trans.frame = 0;
    trans.duration = 0;
    trans.active = FALSE;
    trans.done = FALSE;
}

void trans_start(TransType type, int duration) {
    trans.type = type;
    trans.frame = 0;
    trans.duration = MAX(1, duration);
    trans.active = TRUE;
    trans.done = FALSE;
    
    /* For cross dissolve, capture current BG */
    if (type == TRANS_CROSS_DISSOLVE) {
        u16 *bg_map = BG_SCREENBLOCK(16); /* BG0 screenblock */
        int i;
        for (i = 0; i < 32 * 32; i++) {
            trans.prev_bg[i] = bg_map[i];
        }
    }
}

bool trans_update(void) {
    if (!trans.active) return TRUE;
    
    trans.frame++;
    
    /* Calculate progress (0.0 to 1.0) */
    int progress;
    if (trans.frame >= trans.duration) {
        progress = 16;
    } else {
        progress = (trans.frame * 16) / trans.duration;
    }
    
    switch (trans.type) {
        case TRANS_FADE_OUT:
            /* Fade to black using BLDY */
            REG_BLDCNT = BLD_BLACK | BLD_BG0 | BLD_BG1 | BLD_BG2 | BLD_BG3;
            REG_BLDY = progress;
            break;
            
        case TRANS_FADE_IN:
            /* Fade from black */
            REG_BLDCNT = BLD_BLACK | BLD_BG0 | BLD_BG1 | BLD_BG2 | BLD_BG3;
            REG_BLDY = 16 - progress;
            if (progress >= 16) {
                REG_BLDCNT = 0;
                REG_BLDY = 0;
            }
            break;
            
        case TRANS_WHITE_OUT:
            REG_BLDCNT = BLD_WHITE | BLD_BG0 | BLD_BG1 | BLD_BG2 | BLD_BG3;
            REG_BLDY = progress;
            break;
            
        case TRANS_WHITE_IN:
            REG_BLDCNT = BLD_WHITE | BLD_BG0 | BLD_BG1 | BLD_BG2 | BLD_BG3;
            REG_BLDY = 16 - progress;
            if (progress >= 16) {
                REG_BLDCNT = 0;
                REG_BLDY = 0;
            }
            break;
            
        case TRANS_CROSS_DISSOLVE:
            /* Use alpha blending for dissolve effect */
            REG_BLDCNT = BLD_ALPHA | BLD_BG1 | BLD_BG0;
            REG_BLDALPHA = (16 - progress) | (progress << 8);
            /* BG0 = new scene, BG1 = old scene (stored in prev_bg) */
            /* We blend BG1 (old) onto BG0 (new) */
            if (progress >= 16) {
                REG_BLDCNT = 0;
                REG_BLDALPHA = 0;
            }
            break;
            
        default:
            break;
    }
    
    if (trans.frame >= trans.duration) {
        trans.active = FALSE;
        trans.done = TRUE;
        trans.type = TRANS_NONE;
        /* Reset blending */
        REG_BLDCNT = 0;
        REG_BLDY = 0;
        REG_BLDALPHA = 0;
        return TRUE;
    }
    
    return FALSE;
}

bool trans_active(void) {
    return trans.active;
}

bool trans_done(void) {
    return trans.done;
}

int trans_get_fade_level(void) {
    if (!trans.active) return 0;
    if (trans.frame >= trans.duration) return 16;
    return (trans.frame * 16) / trans.duration;
}

void trans_apply_fade(int level) {
    if (level <= 0) {
        REG_BLDCNT = 0;
        REG_BLDY = 0;
    } else {
        REG_BLDCNT = BLD_BLACK | BLD_BG0 | BLD_BG1 | BLD_BG2 | BLD_BG3;
        REG_BLDY = MIN(level, 16);
    }
}

void trans_fade_out(int duration) {
    trans_start(TRANS_FADE_OUT, duration);
}

void trans_fade_in(int duration) {
    trans_start(TRANS_FADE_IN, duration);
}

void trans_cross_dissolve_start(int duration) {
    trans_start(TRANS_CROSS_DISSOLVE, duration);
}

void trans_cross_dissolve_load_new(void) {
    /* The caller should load the new BG to BG0 after this */
    /* The old BG is already captured in trans.prev_bg */
    /* For now, this is a no-op placeholder for the cross-dissolve */
}