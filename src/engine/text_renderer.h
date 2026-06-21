#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "gba_types.h"

/* Text speed levels */
typedef enum {
    TEXT_SPEED_SLOW = 6,
    TEXT_SPEED_NORMAL = 3,
    TEXT_SPEED_FAST = 1,
    TEXT_SPEED_INSTANT = 0
} TextSpeed;

/* Text colors (palette indices) */
#define TEXT_COLOR_WHITE   0
#define TEXT_COLOR_YELLOW  1
#define TEXT_COLOR_CYAN    2
#define TEXT_COLOR_RED     3
#define TEXT_COLOR_GREEN   4
#define TEXT_COLOR_GRAY    5
#define TEXT_COLOR_ORANGE  6
#define TEXT_COLOR_MAGENTA 7

/* Initialize text rendering system on BG_DIALOG layer */
void vn_text_init(void);

/* Set text speed */
void vn_text_set_speed(TextSpeed speed);

/* Get current text speed */
TextSpeed vn_text_get_speed(void);

/* Draw a single character at tile position (col, row) on BG3 */
void vn_text_draw_char(int col, int row, char ch, u16 color);

/* Draw a string at tile position, no wrapping */
void vn_text_draw_string(int col, int row, const char *str, u16 color);

/* Clear the text area (rows 16-19, the bottom 48px of BG3) */
void vn_text_clear_area(void);

/* Clear entire BG3 tile map */
void vn_text_clear_all(void);

/* Typewriter state */
typedef struct {
    char buffer[256];       /* Full text to display */
    int display_len;        /* Characters currently shown */
    int total_len;          /* Total characters in buffer */
    int cursor_col;         /* Current draw column */
    int cursor_row;         /* Current draw row */
    int start_col;          /* Starting column */
    int start_row;          /* Starting row */
    int max_cols;           /* Max columns before wrap */
    int max_rows;           /* Max rows */
    u16 color;              /* Text color */
    u16 speed_counter;      /* Frames between characters */
    u16 speed_timer;        /* Current speed timer */
    bool complete;          /* Is the full text shown? */
    bool active;            /* Is typewriter running? */
} TypewriterState;

/* Initialize typewriter for a new text */
void vn_typewriter_start(TypewriterState *tw, const char *text, int col, int row,
                         int max_cols, int max_rows, u16 color);

/* Update typewriter (call once per frame), returns true when done */
bool vn_typewriter_update(TypewriterState *tw);

/* Skip typewriter to end (show all text) */
void vn_typewriter_skip(TypewriterState *tw);

/* Check if typewriter is complete */
bool vn_typewriter_is_complete(TypewriterState *tw);

/* Draw text word-wrapped within a region */
void vn_text_draw_wrapped(int col, int row, const char *text, int max_cols,
                          int max_rows, u16 color);

/* Get width of a string in tiles */
int vn_text_width(const char *str);

/* Calculate number of rows needed for wrapped text */
int vn_text_wrapped_rows(const char *text, int max_cols);

#endif /* TEXT_RENDERER_H */