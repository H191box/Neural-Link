#ifndef SCRIPT_ENGINE_H
#define SCRIPT_ENGINE_H

#include "gba_types.h"

/* Script command types */
typedef enum {
    CMD_TEXT,
    CMD_BG,
    CMD_SHOW,
    CMD_HIDE,
    CMD_CHOICE,
    CMD_LABEL,
    CMD_JUMP,
    CMD_SET_FLAG,
    CMD_IF_FLAG,
    CMD_ADD_REP,
    CMD_ADD_AFFINITY,
    CMD_PLAY_SFX,
    CMD_FADE_OUT,
    CMD_FADE_IN,
    CMD_CHAPTER,
    CMD_HACKING,
    CMD_END,
    CMD_CG_UNLOCK,
    CMD_WAIT,
    CMD_SHAKE,
    CMD_NONE
} ScriptCmd;

/* Parsed script command */
typedef struct {
    ScriptCmd cmd;
    char str_params[4][40]; /* Up to 4 string parameters */
    int int_params[4];      /* Up to 4 integer parameters */
} ScriptCommand;

/* Script engine state */
typedef struct {
    const char **lines;      /* Pointer to script line array */
    int total_lines;
    int current_line;
    bool running;
    bool waiting;            /* Waiting for player input (dialog, choice) */
    bool choice_pending;     /* Waiting for choice selection */
    int pending_choice;      /* Selected choice index */
    int chapter;             /* Current chapter (0-4) */
} ScriptState;

/* Initialize script engine */
void script_init(void);

/* Load a script (array of string lines) */
void script_load(const char **lines, int count);

/* Run the next line of script (call until waiting/done) */
void script_run_line(void);

/* Handle choice selection from player */
void script_make_choice(int choice_idx);

/* Jump to a named label */
void script_jump_label(const char *label);

/* Set a story flag */
void script_set_flag(const char *name, bool value);

/* Check a story flag */
bool script_check_flag(const char *name);

/* Get current script state */
const ScriptState* script_get_state(void);

/* Check if script is still running */
bool script_is_running(void);

/* Check if script is waiting for input */
bool script_is_waiting(void);

/* Get current chapter */
int script_get_chapter(void);

/* Parse a single line into a command */
ScriptCommand script_parse_line(const char *line);

#endif /* SCRIPT_ENGINE_H */