#include "text_renderer.h"
#include "bg_manager.h"
#include "gba_header.h"
#include "dma.h"

/* Font data is 8x8 1bpp, stored externally in font.c */
extern const u8 font_data_8x8[95][8];

/* Text system state */
static TextSpeed current_speed = TEXT_SPEED_NORMAL;

/* BG3 tile map for text rendering */
/* Using charblock 3, screenblock 19 */
#define TEXT_CHARBLOCK 3
#define TEXT_SCREENBLOCK 19
#define TEXT_BASE_TILE 1  /* Tile 0 = space/empty */

/* Dialog area: bottom 6 rows (rows 14-19) = 48 pixels, but we use rows 14-19 for flexibility */
#define DIALOG_ROW_START 14
#define DIALOG_COL_START 2
#define DIALOG_COLS 28
#define DIALOG_ROWS 6

/* Get pointer to tile data in VRAM (charblock 3) */
#define TEXT_TILE_VRAM BG_CHARBLOCK(TEXT_CHARBLOCK)

/* Get pointer to tile map in VRAM (screenblock 19) */
#define TEXT_MAP_VRAM BG_SCREENBLOCK(TEXT_SCREENBLOCK)

void vn_text_init(void) {
    current_speed = TEXT_SPEED_NORMAL;
    
    /* Load font into charblock 3 */
    /* Each character is 8x8 1bpp = 8 bytes, expanded to 4bpp = 32 bytes (16 halfwords) */
    u16 *tile_dest = TEXT_TILE_VRAM;
    int ch;
    for (ch = 0; ch < 95; ch++) {
        /* Tile 0 = blank (already zeroed) */
        /* Tiles 1-95 = ASCII 0x20-0x7E */
        const u8 *src = font_data_8x8[ch];
        int row;
        for (row = 0; row < 8; row++) {
            u8 line = src[row];
            /* Expand 1bpp to 4bpp: each bit becomes 4 bits of palette index */
            u16 expanded = 0;
            int bit;
            for (bit = 0; bit < 8; bit++) {
                if (line & (0x80 >> bit)) {
                    /* Set to palette index 1 (will use sub-palette for color) */
                    /* For color text we set individual pixel palette indices */
                    expanded |= (1 << (bit * 2));
                }
            }
            /* Store as two 8-pixel 4bpp values = 4 halfwords per row */
            *tile_dest++ = expanded & 0xFF;         /* Lower 4 pixels */
            *tile_dest++ = (expanded >> 8) & 0xFF;  /* Upper 4 pixels */
        }
    }
    
    /* Clear text tile map */
    vn_text_clear_all();
}

void vn_text_set_speed(TextSpeed speed) {
    current_speed = speed;
}

TextSpeed vn_text_get_speed(void) {
    return current_speed;
}

/* Draw a single character at tile position on BG3 text map */
void vn_text_draw_char(int col, int row, char ch, u16 color) {
    u16 *map = TEXT_MAP_VRAM;
    
    if (row < 0 || row >= TILE_MAP_H || col < 0 || col >= TILE_MAP_W) return;
    
    /* Map ASCII to tile index: space (0x20) = tile 1, '!' = tile 2, etc. */
    u16 tile_idx = 0;
    if (ch >= 0x20 && ch <= 0x7E) {
        tile_idx = (ch - 0x20) + 1;
    }
    
    /* Set tile with palette (color index in upper 4 bits) */
    map[row * TILE_MAP_W + col] = tile_idx | (color << 12);
}

void vn_text_draw_string(int col, int row, const char *str, u16 color) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            row++;
            col = 0; /* Reset to start of line */
        } else {
            vn_text_draw_char(col, row, str[i], color);
            col++;
        }
        i++;
    }
}

void vn_text_clear_area(void) {
    /* Clear dialog rows (14-19) */
    u16 *map = TEXT_MAP_VRAM;
    int r, c;
    for (r = DIALOG_ROW_START; r < TILE_MAP_H; r++) {
        for (c = 0; c < TILE_MAP_W; c++) {
            map[r * TILE_MAP_W + c] = 0;
        }
    }
}

void vn_text_clear_all(void) {
    u16 *map = TEXT_MAP_VRAM;
    int i;
    for (i = 0; i < BG_TILE_MAP_SIZE; i++) {
        map[i] = 0;
    }
}

void vn_typewriter_start(TypewriterState *tw, const char *text, int col, int row,
                         int max_cols, int max_rows, u16 color) {
    int i;
    /* Copy text to buffer */
    for (i = 0; i < 255 && text[i] != '\0'; i++) {
        tw->buffer[i] = text[i];
    }
    tw->buffer[i] = '\0';
    
    tw->total_len = i;
    tw->display_len = 0;
    tw->cursor_col = col;
    tw->cursor_row = row;
    tw->start_col = col;
    tw->start_row = row;
    tw->max_cols = max_cols;
    tw->max_rows = max_rows;
    tw->color = color;
    tw->speed_counter = (current_speed == TEXT_SPEED_INSTANT) ? 0 : current_speed;
    tw->speed_timer = 0;
    tw->complete = (current_speed == TEXT_SPEED_INSTANT);
    tw->active = TRUE;
    
    /* If instant, show all immediately */
    if (current_speed == TEXT_SPEED_INSTANT) {
        vn_typewriter_skip(tw);
    }
}

bool vn_typewriter_update(TypewriterState *tw) {
    if (!tw->active || tw->complete) return TRUE;
    
    tw->speed_timer++;
    if (tw->speed_timer >= tw->speed_counter) {
        tw->speed_timer = 0;
        
        if (tw->display_len < tw->total_len) {
            char ch = tw->buffer[tw->display_len];
            
            if (ch == '\n') {
                tw->cursor_row++;
                tw->cursor_col = tw->start_col;
            } else {
                vn_text_draw_char(tw->cursor_col, tw->cursor_row, ch, tw->color);
                tw->cursor_col++;
                
                /* Word wrap */
                if (tw->cursor_col >= tw->start_col + tw->max_cols) {
                    tw->cursor_col = tw->start_col;
                    tw->cursor_row++;
                }
            }
            
            tw->display_len++;
            
            /* Check if we've filled the box */
            if (tw->cursor_row >= tw->start_row + tw->max_rows) {
                tw->complete = TRUE;
                tw->active = FALSE;
                return TRUE;
            }
        }
        
        if (tw->display_len >= tw->total_len) {
            tw->complete = TRUE;
            tw->active = FALSE;
            return TRUE;
        }
    }
    
    return tw->complete;
}

void vn_typewriter_skip(TypewriterState *tw) {
    while (tw->display_len < tw->total_len) {
        char ch = tw->buffer[tw->display_len];
        
        if (ch == '\n') {
            tw->cursor_row++;
            tw->cursor_col = tw->start_col;
        } else {
            vn_text_draw_char(tw->cursor_col, tw->cursor_row, ch, tw->color);
            tw->cursor_col++;
            
            if (tw->cursor_col >= tw->start_col + tw->max_cols) {
                tw->cursor_col = tw->start_col;
                tw->cursor_row++;
            }
        }
        
        tw->display_len++;
    }
    
    tw->complete = TRUE;
    tw->active = FALSE;
}

bool vn_typewriter_is_complete(TypewriterState *tw) {
    return tw->complete;
}

void vn_text_draw_wrapped(int col, int row, const char *text, int max_cols,
                          int max_rows, u16 color) {
    int word_start = 0;
    int line_col = 0;
    int line_row = 0;
    int i = 0;
    
    while (text[i] != '\0' && line_row < max_rows) {
        if (text[i] == ' ') {
            /* Look ahead to see if next word fits */
            int word_len = 0;
            int j = i + 1;
            while (text[j] != '\0' && text[j] != ' ' && text[j] != '\n') {
                word_len++;
                j++;
            }
            
            if (line_col + word_len > max_cols && line_col > 0) {
                /* Wrap */
                line_col = 0;
                line_row++;
                if (line_row >= max_rows) break;
            }
            
            i++; /* Skip space */
        } else if (text[i] == '\n') {
            line_col = 0;
            line_row++;
            i++;
            if (line_row >= max_rows) break;
        } else {
            vn_text_draw_char(col + line_col, row + line_row, text[i], color);
            line_col++;
            i++;
        }
    }
}

int vn_text_width(const char *str) {
    int len = 0;
    while (str[len] != '\0' && str[len] != '\n') len++;
    return len;
}

int vn_text_wrapped_rows(const char *text, int max_cols) {
    int word_start = 0;
    int line_col = 0;
    int line_row = 0;
    int i = 0;
    
    while (text[i] != '\0') {
        if (text[i] == ' ') {
            int word_len = 0;
            int j = i + 1;
            while (text[j] != '\0' && text[j] != ' ' && text[j] != '\n') {
                word_len++;
                j++;
            }
            if (line_col + word_len > max_cols && line_col > 0) {
                line_col = 0;
                line_row++;
            }
            i++;
        } else if (text[i] == '\n') {
            line_col = 0;
            line_row++;
            i++;
        } else {
            line_col++;
            i++;
        }
    }
    
    return line_row + 1;
}