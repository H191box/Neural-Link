#ifndef HACKING_MINIGAME_H
#define HACKING_MINIGAME_H

#include "gba_types.h"

/* Neural Sync grid size */
#define HACK_GRID_SIZE 5
#define HACK_MAX_NODES (HACK_GRID_SIZE * HACK_GRID_SIZE)

/* Node types */
typedef enum {
    NODE_EMPTY = 0,
    NODE_START,
    NODE_END,
    NODE_PATH,
    NODE_TRAP,
    NODE_SELECTED,
    NODE_CORRECT,
    NODE_WRONG
} NodeType;

/* Neural Sync state */
typedef struct {
    NodeType grid[HACK_GRID_SIZE][HACK_GRID_SIZE];
    int cursor_x;
    int cursor_y;
    int start_x, start_y;
    int end_x, end_y;
    int selected_count;
    int path[HACK_MAX_NODES][2];
    int difficulty;
    int time_limit;     /* In frames (60fps) */
    int time_left;
    bool active;
    bool complete;
    bool success;
    int anim_timer;
    int target_li;      /* Which LI we're syncing with (for story context) */
} HackState;

/* Initialize neural sync minigame with difficulty (1-5) and target LI */
void hack_init(int difficulty);

/* Update neural sync minigame */
void hack_update(void);

/* Render neural sync minigame to screen */
void hack_render(void);

/* Check if sync is complete */
bool hack_is_complete(void);

/* Check if player succeeded */
bool hack_success(void);

/* Set the target LI for flavor text */
void hack_set_target_li(int li_id);

#endif /* HACKING_MINIGAME_H */