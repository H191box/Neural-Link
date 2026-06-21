#include "hacking_minigame.h"
#include "input.h"
#include "bg_manager.h"
#include "text_renderer.h"
#include "system.h"
#include "gba_header.h"
#include "dma.h"

static HackState hack;

/* Simple RNG for puzzle generation */
static u16 hack_rng_state = 12345;
static u16 hack_rng(void) {
    hack_rng_state = hack_rng_state * 1103515245 + 12345;
    return (hack_rng_state >> 16) & 0x7FFF;
}

void hack_init(int difficulty) {
    int x, y;
    
    /* Clear grid */
    for (x = 0; x < HACK_GRID_SIZE; x++) {
        for (y = 0; y < HACK_GRID_SIZE; y++) {
            hack.grid[x][y] = NODE_EMPTY;
        }
    }
    
    hack.difficulty = CLAMP(difficulty, 1, 5);
    hack.selected_count = 0;
    hack.active = TRUE;
    hack.complete = FALSE;
    hack.success = FALSE;
    hack.anim_timer = 0;
    
    /* Set time limit based on difficulty */
    hack.time_limit = 60 * (20 - difficulty * 3); /* 20s down to 5s */
    hack.time_left = hack.time_limit;
    
    /* Place start and end nodes */
    hack.start_x = 0;
    hack.start_y = hack_rng() % HACK_GRID_SIZE;
    hack.end_x = HACK_GRID_SIZE - 1;
    hack.end_y = hack_rng() % HACK_GRID_SIZE;
    
    hack.grid[hack.start_x][hack.start_y] = NODE_START;
    hack.grid[hack.end_x][hack.end_y] = NODE_END;
    
    /* Generate a valid path from start to end */
    int path_x = hack.start_x;
    int path_y = hack.start_y;
    
    /* Simple path: move right and occasionally up/down */
    while (path_x < hack.end_x || path_y != hack.end_y) {
        if (path_x < hack.end_x) {
            path_x++;
        } else if (path_y < hack.end_y) {
            path_y++;
        } else if (path_y > hack.end_y) {
            path_y--;
        }
        
        if (path_x == hack.end_x && path_y == hack.end_y) break;
        if (path_x < HACK_GRID_SIZE && path_y >= 0 && path_y < HACK_GRID_SIZE) {
            hack.grid[path_x][path_y] = NODE_PATH;
        }
    }
    
    /* Place traps based on difficulty */
    int num_traps = difficulty * 3;
    int placed = 0;
    int attempts = 0;
    while (placed < num_traps && attempts < 100) {
        int tx = hack_rng() % HACK_GRID_SIZE;
        int ty = hack_rng() % HACK_GRID_SIZE;
        if (hack.grid[tx][ty] == NODE_EMPTY) {
            hack.grid[tx][ty] = NODE_TRAP;
            placed++;
        }
        attempts++;
    }
    
    /* Set cursor to start */
    hack.cursor_x = hack.start_x;
    hack.cursor_y = hack.start_y;
}

void hack_update(void) {
    if (!hack.active || hack.complete) return;
    
    hack.anim_timer++;
    hack.time_left--;
    
    if (hack.time_left <= 0) {
        hack.complete = TRUE;
        hack.success = FALSE;
        return;
    }
    
    /* Input handling */
    if (input_pressed(KEY_LEFT) && hack.cursor_x > 0) {
        hack.cursor_x--;
        sfx_play(SFX_MENU_MOVE);
    }
    if (input_pressed(KEY_RIGHT) && hack.cursor_x < HACK_GRID_SIZE - 1) {
        hack.cursor_x++;
        sfx_play(SFX_MENU_MOVE);
    }
    if (input_pressed(KEY_UP) && hack.cursor_y > 0) {
        hack.cursor_y--;
        sfx_play(SFX_MENU_MOVE);
    }
    if (input_pressed(KEY_DOWN) && hack.cursor_y < HACK_GRID_SIZE - 1) {
        hack.cursor_y++;
        sfx_play(SFX_MENU_MOVE);
    }
    
    /* Select node with A */
    if (input_pressed(KEY_A)) {
        int cx = hack.cursor_x;
        int cy = hack.cursor_y;
        
        if (hack.grid[cx][cy] == NODE_TRAP) {
            /* Hit a trap! */
            hack.grid[cx][cy] = NODE_WRONG;
            hack.complete = TRUE;
            hack.success = FALSE;
            return;
        }
        
        /* Add to selected path */
        if (hack.selected_count < HACK_MAX_NODES) {
            hack.path[hack.selected_count][0] = cx;
            hack.path[hack.selected_count][1] = cy;
            hack.selected_count++;
            hack.grid[cx][cy] = NODE_SELECTED;
        }
        
        /* Check if reached end */
        if (cx == hack.end_x && cy == hack.end_y) {
            hack.complete = TRUE;
            hack.success = TRUE;
            /* Mark all path nodes as correct */
            int i;
            for (i = 0; i < hack.selected_count; i++) {
                int px = hack.path[i][0];
                int py = hack.path[i][1];
                if (hack.grid[px][py] == NODE_SELECTED) {
                    hack.grid[px][py] = NODE_CORRECT;
                }
            }
        }
    }
    
    /* B to cancel last selection */
    if (input_pressed(KEY_B) && hack.selected_count > 0) {
        hack.selected_count--;
        int px = hack.path[hack.selected_count][0];
        int py = hack.path[hack.selected_count][1];
        /* Restore original node type */
        if (px == hack.start_x && py == hack.start_y) {
            hack.grid[px][py] = NODE_START;
        } else if (px == hack.end_x && py == hack.end_y) {
            hack.grid[px][py] = NODE_END;
        } else {
            hack.grid[px][py] = NODE_PATH;
        }
    }
}

void hack_render(void) {
    int x, y;
    
    /* Clear the screen area (top portion above dialog) */
    for (y = 0; y < 14; y++) {
        for (x = 0; x < 30; x++) {
            bg_set_tile(BG_UI, x, y, 0);
        }
    }
    
    /* Draw title */
    vn_text_draw_string(7, 0, "NEURAL INTRUSION", TEXT_COLOR_RED);
    vn_text_draw_string(5, 1, "CONNECT NODES: START TO END", TEXT_COLOR_CYAN);
    
    /* Draw timer */
    {
        char time_str[12];
        int seconds = hack.time_left / 60;
        int digits = 0;
        if (seconds < 0) seconds = 0;
        
        /* Convert seconds to string */
        if (seconds >= 100) { time_str[digits++] = '0' + (seconds / 100); seconds %= 100; }
        if (seconds >= 10 || digits > 0) { time_str[digits++] = '0' + (seconds / 10); seconds %= 10; }
        time_str[digits++] = '0' + seconds;
        time_str[digits++] = 's';
        time_str[digits] = '\0';
        
        u16 timer_color = (hack.time_left < 180) ? TEXT_COLOR_RED : TEXT_COLOR_YELLOW;
        vn_text_draw_string(22, 0, time_str, timer_color);
    }
    
    /* Draw grid (centered on screen, 5x5 grid of 2x2 tiles = 10x10 tiles, offset to center) */
    int grid_offset_x = 9;
    int grid_offset_y = 3;
    int grid_scale = 2; /* Each node is 2x2 tiles */
    
    for (y = 0; y < HACK_GRID_SIZE; y++) {
        for (x = 0; x < HACK_GRID_SIZE; x++) {
            int screen_x = grid_offset_x + x * grid_scale;
            int screen_y = grid_offset_y + y * grid_scale;
            NodeType node = hack.grid[x][y];
            u16 color = TEXT_COLOR_GRAY;
            
            switch (node) {
                case NODE_EMPTY:
                    color = TEXT_COLOR_GRAY;
                    /* Draw empty cell */
                    bg_set_tile_pal(BG_UI, screen_x, screen_y, 0, 7);
                    bg_set_tile_pal(BG_UI, screen_x+1, screen_y, 0, 7);
                    bg_set_tile_pal(BG_UI, screen_x, screen_y+1, 0, 7);
                    bg_set_tile_pal(BG_UI, screen_x+1, screen_y+1, 0, 7);
                    break;
                case NODE_START:
                    vn_text_draw_char(screen_x, screen_y, 'S', TEXT_COLOR_GREEN);
                    break;
                case NODE_END:
                    vn_text_draw_char(screen_x, screen_y, 'E', TEXT_COLOR_RED);
                    break;
                case NODE_PATH:
                    vn_text_draw_char(screen_x, screen_y, '.', TEXT_COLOR_CYAN);
                    break;
                case NODE_TRAP:
                    vn_text_draw_char(screen_x, screen_y, 'X', TEXT_COLOR_GRAY);
                    break;
                case NODE_SELECTED:
                    vn_text_draw_char(screen_x, screen_y, '#', TEXT_COLOR_YELLOW);
                    break;
                case NODE_CORRECT:
                    vn_text_draw_char(screen_x, screen_y, '#', TEXT_COLOR_GREEN);
                    break;
                case NODE_WRONG:
                    vn_text_draw_char(screen_x, screen_y, '!', TEXT_COLOR_RED);
                    break;
            }
        }
    }
    
    /* Draw cursor */
    if (hack.active && !hack.complete) {
        int cx = grid_offset_x + hack.cursor_x * grid_scale;
        int cy = grid_offset_y + hack.cursor_y * grid_scale;
        if (hack.anim_timer & 8) {
            vn_text_draw_char(cx - 1, cy, '[', TEXT_COLOR_YELLOW);
            vn_text_draw_char(cx + grid_scale, cy, ']', TEXT_COLOR_YELLOW);
        }
    }
    
    /* Draw instructions */
    vn_text_draw_string(2, 12, "A:Select B:Undo", TEXT_COLOR_GRAY);
    
    /* Draw result */
    if (hack.complete) {
        if (hack.success) {
            vn_text_draw_string(7, 13, "ACCESS GRANTED", TEXT_COLOR_GREEN);
        } else {
            vn_text_draw_string(6, 13, "INTRUSION DETECTED", TEXT_COLOR_RED);
        }
    }
}

bool hack_is_complete(void) {
    return hack.complete;
}

bool hack_success(void) {
    return hack.success;
}