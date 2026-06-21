#include "script_engine.h"
#include "dialog_box.h"
#include "transition.h"
#include "backgrounds.h"
#include "faction.h"
#include "flags.h"
#include "characters_game.h"
#include "characters.h"
#include "audio.h"
#include "sprite_manager.h"
#include "bg_manager.h"
#include "text_renderer.h"
#include "novel_state.h"
#include "hacking_minigame.h"
#include "settings.h"
#include "system.h"

static ScriptState state;

/* Pending commands queue (for multi-step operations) */
static ScriptCommand pending_cmd;
static bool has_pending = FALSE;

/* Helper: parse next word from string, advance pointer */
static int parse_word(const char *str, char *out, int max_len) {
    int i = 0;
    /* Skip leading whitespace */
    while (*str == ' ' || *str == '\t') str++;
    /* Skip if starts with quote */
    bool in_quotes = FALSE;
    if (*str == '"') {
        str++;
        in_quotes = TRUE;
    }
    /* Read until space/end/quote */
    while (*str != '\0' && *str != '\n' && i < max_len - 1) {
        if (in_quotes && *str == '"') {
            str++; /* Skip closing quote */
            break;
        }
        if (!in_quotes && (*str == ' ' || *str == '\t')) break;
        out[i++] = *str++;
    }
    out[i] = '\0';
    return i;
}

/* Helper: find label line number */
static int find_label(const char *label) {
    int i;
    char cmd[20];
    char param[40];
    for (i = 0; i < state.total_lines; i++) {
        const char *line = state.lines[i];
        if (line[0] == '#' || line[0] == '\0') continue; /* Comment/empty */
        parse_word(line, cmd, sizeof(cmd));
        if (my_strcmp(cmd, "LABEL") == 0) {
            const char *p = line;
            /* Skip "LABEL " */
            while (*p && *p != ' ') p++;
            while (*p == ' ') p++;
            parse_word(p, param, sizeof(param));
            if (my_strcmp(param, label) == 0) {
                return i;
            }
        }
    }
    return -1; /* Not found */
}

/* Helper: extract quoted string from line */
static void extract_quoted(const char *src, char *dst, int max_len) {
    int i = 0;
    /* Find opening quote */
    while (*src && *src != '"') src++;
    if (*src == '"') src++; /* Skip quote */
    while (*src && *src != '"' && i < max_len - 1) {
        dst[i++] = *src++;
    }
    dst[i] = '\0';
}

/* Helper: extract integer from string after keyword */
static int extract_int(const char *src) {
    /* Find first digit */
    while (*src && (*src < '0' || *src > '9') && *src != '-') src++;
    int val = 0;
    int neg = 1;
    if (*src == '-') { neg = -1; src++; }
    while (*src >= '0' && *src <= '9') {
        val = val * 10 + (*src - '0');
        src++;
    }
    return val * neg;
}

ScriptCommand script_parse_line(const char *line) {
    ScriptCommand cmd;
    int i;
    for (i = 0; i < 4; i++) {
        cmd.str_params[i][0] = '\0';
        cmd.int_params[i] = 0;
    }
    cmd.cmd = CMD_NONE;
    
    if (line == NULL || line[0] == '\0' || line[0] == '#') {
        return cmd;
    }
    
    char keyword[20];
    const char *p = line;
    parse_word(p, keyword, sizeof(keyword));
    
    if (my_strcmp(keyword, "TEXT") == 0) {
        cmd.cmd = CMD_TEXT;
        /* Format: TEXT "speaker" "dialog text" */
        p += my_strlen("TEXT");
        extract_quoted(p, cmd.str_params[0], 40); /* speaker */
        /* Find second quote */
        const char *q = p;
        int quote_count = 0;
        while (*q && quote_count < 2) {
            if (*q == '"') quote_count++;
            q++;
        }
        if (quote_count >= 2) extract_quoted(q - 1, cmd.str_params[1], 40); /* text */
    }
    else if (my_strcmp(keyword, "BG") == 0) {
        cmd.cmd = CMD_BG;
        p += my_strlen("BG");
        parse_word(p, cmd.str_params[0], 40);
        /* Convert background name to ID */
        if (my_strcmp(cmd.str_params[0], "cityscape") == 0) cmd.int_params[0] = BG_ID_CITYSCAPE;
        else if (my_strcmp(cmd.str_params[0], "lab") == 0) cmd.int_params[0] = BG_ID_LAB;
        else if (my_strcmp(cmd.str_params[0], "office") == 0) cmd.int_params[0] = BG_ID_MEGACORP;
        else if (my_strcmp(cmd.str_params[0], "base") == 0) cmd.int_params[0] = BG_ID_RESISTANCE;
        else if (my_strcmp(cmd.str_params[0], "network") == 0) cmd.int_params[0] = BG_ID_NETWORK;
    }
    else if (my_strcmp(keyword, "SHOW") == 0) {
        cmd.cmd = CMD_SHOW;
        p += my_strlen("SHOW");
        parse_word(p, cmd.str_params[0], 40);
        parse_word(p + my_strlen(cmd.str_params[0]), cmd.str_params[1], 40);
        /* Convert char name to ID */
        if (my_strcmp(cmd.str_params[0], "Kira") == 0) cmd.int_params[0] = GC_KIRA;
        else if (my_strcmp(cmd.str_params[0], "Nova") == 0) cmd.int_params[0] = GC_NOVA;
        else if (my_strcmp(cmd.str_params[0], "Chen") == 0) cmd.int_params[0] = GC_CHEN;
        else if (my_strcmp(cmd.str_params[0], "ARIA") == 0) cmd.int_params[0] = GC_ARIA;
        else if (my_strcmp(cmd.str_params[0], "Vex") == 0) cmd.int_params[0] = GC_VEX;
    }
    else if (my_strcmp(keyword, "HIDE") == 0) {
        cmd.cmd = CMD_HIDE;
        p += my_strlen("HIDE");
        parse_word(p, cmd.str_params[0], 40);
        if (my_strcmp(cmd.str_params[0], "Kira") == 0) cmd.int_params[0] = GC_KIRA;
        else if (my_strcmp(cmd.str_params[0], "Nova") == 0) cmd.int_params[0] = GC_NOVA;
        else if (my_strcmp(cmd.str_params[0], "Chen") == 0) cmd.int_params[0] = GC_CHEN;
        else if (my_strcmp(cmd.str_params[0], "ARIA") == 0) cmd.int_params[0] = GC_ARIA;
        else if (my_strcmp(cmd.str_params[0], "Vex") == 0) cmd.int_params[0] = GC_VEX;
    }
    else if (my_strcmp(keyword, "CHOICE") == 0) {
        cmd.cmd = CMD_CHOICE;
        p += my_strlen("CHOICE");
        /* Parse multiple quoted strings as choices */
        int ci = 0;
        while (*p && ci < 4) {
            while (*p && *p != '"') p++;
            if (!*p) break;
            extract_quoted(p, cmd.str_params[ci], 40);
            if (cmd.str_params[ci][0] != '\0') ci++;
            /* Move past this quote */
            int qc = 0;
            while (*p && qc < 2) { if (*p == '"') qc++; p++; }
        }
        cmd.int_params[0] = ci; /* Number of choices */
    }
    else if (my_strcmp(keyword, "LABEL") == 0) {
        cmd.cmd = CMD_LABEL;
        p += my_strlen("LABEL");
        parse_word(p, cmd.str_params[0], 40);
    }
    else if (my_strcmp(keyword, "JUMP") == 0) {
        cmd.cmd = CMD_JUMP;
        p += my_strlen("JUMP");
        parse_word(p, cmd.str_params[0], 40);
    }
    else if (my_strcmp(keyword, "SET_FLAG") == 0) {
        cmd.cmd = CMD_SET_FLAG;
        p += my_strlen("SET_FLAG");
        extract_quoted(p, cmd.str_params[0], 40);
    }
    else if (my_strcmp(keyword, "IF_FLAG") == 0) {
        cmd.cmd = CMD_IF_FLAG;
        p += my_strlen("IF_FLAG");
        extract_quoted(p, cmd.str_params[0], 40); /* flag name */
        /* Find label */
        const char *q = p;
        int qc = 0;
        while (*q && qc < 2) { if (*q == '"') qc++; q++; }
        parse_word(q, cmd.str_params[1], 40); /* label */
    }
    else if (my_strcmp(keyword, "ADD_REP") == 0) {
        cmd.cmd = CMD_ADD_REP;
        p += my_strlen("ADD_REP");
        parse_word(p, cmd.str_params[0], 40);
        cmd.int_params[1] = extract_int(p + my_strlen(cmd.str_params[0]));
        /* Convert faction name */
        if (my_strcmp(cmd.str_params[0], "resistance") == 0) cmd.int_params[0] = FACTION_RESISTANCE;
        else if (my_strcmp(cmd.str_params[0], "megacorp") == 0) cmd.int_params[0] = FACTION_MEGACORP;
        else if (my_strcmp(cmd.str_params[0], "neutrals") == 0) cmd.int_params[0] = FACTION_NEUTRALS;
        else if (my_strcmp(cmd.str_params[0], "scientists") == 0) cmd.int_params[0] = FACTION_NEUTRAL_SCIENTISTS;
        else if (my_strcmp(cmd.str_params[0], "ai_collective") == 0) cmd.int_params[0] = FACTION_AI_COLLECTIVE;
    }
    else if (my_strcmp(keyword, "ADD_AFFINITY") == 0) {
        cmd.cmd = CMD_ADD_AFFINITY;
        p += my_strlen("ADD_AFFINITY");
        parse_word(p, cmd.str_params[0], 40);
        cmd.int_params[1] = extract_int(p + my_strlen(cmd.str_params[0]));
        if (my_strcmp(cmd.str_params[0], "Kira") == 0) cmd.int_params[0] = GC_KIRA;
        else if (my_strcmp(cmd.str_params[0], "Nova") == 0) cmd.int_params[0] = GC_NOVA;
        else if (my_strcmp(cmd.str_params[0], "Chen") == 0) cmd.int_params[0] = GC_CHEN;
        else if (my_strcmp(cmd.str_params[0], "ARIA") == 0) cmd.int_params[0] = GC_ARIA;
        else if (my_strcmp(cmd.str_params[0], "Vex") == 0) cmd.int_params[0] = GC_VEX;
    }
    else if (my_strcmp(keyword, "PLAY_SFX") == 0) {
        cmd.cmd = CMD_PLAY_SFX;
        p += my_strlen("PLAY_SFX");
        parse_word(p, cmd.str_params[0], 40);
        if (my_strcmp(cmd.str_params[0], "text") == 0) cmd.int_params[0] = SFX_TEXT_ADVANCE;
        else if (my_strcmp(cmd.str_params[0], "choice") == 0) cmd.int_params[0] = SFX_CHOICE_SELECT;
        else if (my_strcmp(cmd.str_params[0], "menu") == 0) cmd.int_params[0] = SFX_MENU_MOVE;
        else if (my_strcmp(cmd.str_params[0], "hack_ok") == 0) cmd.int_params[0] = SFX_HACK_SUCCESS;
        else if (my_strcmp(cmd.str_params[0], "hack_fail") == 0) cmd.int_params[0] = SFX_HACK_FAIL;
    }
    else if (my_strcmp(keyword, "FADE_OUT") == 0) {
        cmd.cmd = CMD_FADE_OUT;
        cmd.int_params[0] = 30; /* Default 30 frames */
    }
    else if (my_strcmp(keyword, "FADE_IN") == 0) {
        cmd.cmd = CMD_FADE_IN;
        cmd.int_params[0] = 30;
    }
    else if (my_strcmp(keyword, "CHAPTER") == 0) {
        cmd.cmd = CMD_CHAPTER;
        p += my_strlen("CHAPTER");
        extract_quoted(p, cmd.str_params[0], 40);
        cmd.int_params[0] = extract_int(p + 40);
    }
    else if (my_strcmp(keyword, "HACKING") == 0) {
        cmd.cmd = CMD_HACKING;
        p += my_strlen("HACKING");
        cmd.int_params[0] = extract_int(p);
    }
    else if (my_strcmp(keyword, "END") == 0) {
        cmd.cmd = CMD_END;
    }
    else if (my_strcmp(keyword, "CG_UNLOCK") == 0) {
        cmd.cmd = CMD_CG_UNLOCK;
        p += my_strlen("CG_UNLOCK");
        extract_quoted(p, cmd.str_params[0], 40);
        cmd.int_params[0] = extract_int(p + 40);
    }
    else if (my_strcmp(keyword, "WAIT") == 0) {
        cmd.cmd = CMD_WAIT;
        p += my_strlen("WAIT");
        cmd.int_params[0] = extract_int(p);
    }
    
    return cmd;
}

void script_init(void) {
    state.lines = NULL;
    state.total_lines = 0;
    state.current_line = 0;
    state.running = FALSE;
    state.waiting = FALSE;
    state.choice_pending = FALSE;
    state.pending_choice = -1;
    state.chapter = 0;
    has_pending = FALSE;
}

void script_load(const char **lines, int count) {
    state.lines = lines;
    state.total_lines = count;
    state.current_line = 0;
    state.running = TRUE;
    state.waiting = FALSE;
    state.choice_pending = FALSE;
    state.pending_choice = -1;
    has_pending = FALSE;
}

/* Show a character sprite on BG1 */
static void show_character_sprite(int char_id, const char *position) {
    int x = 80;
    int y = 16;
    
    if (my_strcmp(position, "left") == 0) x = 16;
    else if (my_strcmp(position, "right") == 0) x = 160;
    else if (my_strcmp(position, "center") == 0) x = 80;
    
    /* Use sprite slot 0 for displayed character */
    spr_set(0, x, y, char_id * 32, character_get_palette(char_id),
            SPR_SQUARE, SPR_SIZE_32x32, 1);
    spr_update();
}

static void hide_character_sprite(void) {
    spr_hide(0);
    spr_update();
}

void script_run_line(void) {
    if (!state.running || state.waiting) return;
    if (state.current_line >= state.total_lines) {
        state.running = FALSE;
        return;
    }
    
    ScriptCommand cmd = script_parse_line(state.lines[state.current_line]);
    
    switch (cmd.cmd) {
        case CMD_NONE:
        case CMD_LABEL:
            /* Skip - labels and comments do nothing */
            state.current_line++;
            break;
            
        case CMD_TEXT: {
            dialog_open();
            dialog_set_speaker(cmd.str_params[0]);
            dialog_set_text(cmd.str_params[1]);
            state.waiting = TRUE;
            state.current_line++;
            sfx_play(SFX_TEXT_ADVANCE);
            break;
        }
        
        case CMD_BG: {
            /* Fade out, change BG, fade in */
            if (!trans_active()) {
                trans_start(TRANS_FADE_OUT, 15);
                has_pending = TRUE;
                pending_cmd = cmd;
            }
            break;
        }
        
        case CMD_SHOW:
            show_character_sprite(cmd.int_params[0], cmd.str_params[1]);
            state.current_line++;
            break;
            
        case CMD_HIDE:
            hide_character_sprite();
            state.current_line++;
            break;
            
        case CMD_CHOICE: {
            int i;
            dialog_clear_choices();
            for (i = 0; i < cmd.int_params[0]; i++) {
                dialog_add_choice(cmd.str_params[i], i);
            }
            state.waiting = TRUE;
            state.choice_pending = TRUE;
            state.current_line++;
            break;
        }
        
        case CMD_JUMP: {
            int target = find_label(cmd.str_params[0]);
            if (target >= 0) {
                state.current_line = target;
            } else {
                state.current_line++; /* Skip if label not found */
            }
            break;
        }
        
        case CMD_SET_FLAG:
            flag_set(cmd.str_params[0], TRUE);
            state.current_line++;
            break;
            
        case CMD_IF_FLAG:
            if (flag_get(cmd.str_params[0])) {
                int target = find_label(cmd.str_params[1]);
                if (target >= 0) state.current_line = target;
                else state.current_line++;
            } else {
                state.current_line++;
            }
            break;
            
        case CMD_ADD_REP:
            faction_modify(cmd.int_params[0], (s16)cmd.int_params[1]);
            state.current_line++;
            break;
            
        case CMD_ADD_AFFINITY:
            game_chars_modify_affinity(cmd.int_params[0], (s16)cmd.int_params[1]);
            state.current_line++;
            break;
            
        case CMD_PLAY_SFX:
            sfx_play(cmd.int_params[0]);
            state.current_line++;
            break;
            
        case CMD_FADE_OUT:
            if (!trans_active()) {
                trans_start(TRANS_FADE_OUT, cmd.int_params[0] > 0 ? cmd.int_params[0] : 30);
                state.waiting = TRUE;
            }
            break;
            
        case CMD_FADE_IN:
            if (!trans_active()) {
                trans_start(TRANS_FADE_IN, cmd.int_params[0] > 0 ? cmd.int_params[0] : 30);
                state.waiting = TRUE;
            }
            break;
            
        case CMD_CHAPTER: {
            /* Set chapter title and trigger chapter intro state */
            novel_state_set_chapter_title(cmd.str_params[0]);
            novel_state_set_state(STATE_CHAPTER_INTRO);
            state.chapter = cmd.int_params[0] > 0 ? cmd.int_params[0] - 1 : state.chapter + 1;
            state.current_line++;
            break;
        }
        
        case CMD_HACKING: {
            int diff = cmd.int_params[0] > 0 ? cmd.int_params[0] : 1;
            hack_init(diff);
            novel_state_set_state(STATE_HACKING);
            state.waiting = TRUE;
            state.current_line++;
            break;
        }
        
        case CMD_END:
            state.running = FALSE;
            novel_state_set_state(STATE_ENDING);
            break;
            
        case CMD_CG_UNLOCK:
            /* Stub: unlock CG in gallery */
            state.current_line++;
            break;
            
        case CMD_WAIT:
            /* Wait N frames - use a simple counter */
            state.waiting = TRUE;
            state.current_line++;
            break;
    }
}

/* Handle pending commands (e.g., BG change after fade out) */
void script_handle_pending(void) {
    if (!has_pending) return;
    if (!trans_active() && trans_done()) {
        /* Execute the pending BG change */
        if (pending_cmd.cmd == CMD_BG) {
            backgrounds_load(pending_cmd.int_params[0]);
            trans_start(TRANS_FADE_IN, 15);
        }
        has_pending = FALSE;
        state.current_line++; /* Advance past the BG command */
    }
}

void script_make_choice(int choice_idx) {
    state.pending_choice = choice_idx;
    state.choice_pending = FALSE;
    state.waiting = FALSE;
}

void script_jump_label(const char *label) {
    int target = find_label(label);
    if (target >= 0) state.current_line = target;
}

void script_set_flag(const char *name, bool value) {
    flag_set(name, value);
}

bool script_check_flag(const char *name) {
    return flag_get(name);
}

const ScriptState* script_get_state(void) {
    return &state;
}

bool script_is_running(void) {
    return state.running;
}

bool script_is_waiting(void) {
    return state.waiting;
}

int script_get_chapter(void) {
    return state.chapter;
}