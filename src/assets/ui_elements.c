#include "ui_elements.h"
#include "bg_manager.h"
#include "dma.h"
#include "gba_header.h"

/*
 * UI tile data for dialog boxes, menus, etc.
 * 32 tiles, each 16 u16 (4bpp, 8x8).
 * Loaded to BG2 charblock (charblock 2).
 * Uses palette sub-palette 1 (indices 16-31) for UI colors.
 * 
 * Tile layout:
 * 0: Fill (semi-transparent dark)
 * 1: Top edge
 * 2: Bottom edge
 * 3: Left edge
 * 4: Right edge
 * 5: Top-left corner
 * 6: Top-right corner
 * 7: Bottom-left corner
 * 8: Bottom-right corner
 * 9: Arrow cursor (for choices)
 * 10: Selection highlight
 * 11-15: Various button tiles
 * 16-31: Title screen and menu tiles
 */

/*
 * Generate UI tiles procedurally.
 * Each tile = 16 u16 values (4bpp 8x8).
 * Palette 1 = UI palette (dark BGs, cyan accents, white text)
 */
const u16 ui_tiles[32 * 16] = {0}; /* 32 tiles * 16 halfwords */

/* Generate UI tiles at init */
static u16 ui_gen[32 * 16];

static void make_ui_tiles(void) {
    int t, i;
    
    /* Clear all */
    for (i = 0; i < 32 * 16; i++) ui_gen[i] = 0;
    
    /* Tile 0: Fill tile - dark semi-transparent look */
    /* Using palette index 1 (dark panel BG) */
    {
        u16 fill = 1 | (1 << 2) | (1 << 4) | (1 << 6) | (1 << 8) | (1 << 10) | (1 << 12) | (1 << 14);
        for (i = 0; i < 16; i++) ui_gen[0 * 16 + i] = fill;
    }
    
    /* Tile 1: Top edge - fill with bright border on top */
    {
        for (i = 0; i < 16; i++) ui_gen[1 * 16 + i] = 1 | (1<<2) | (1<<4) | (1<<6) | (1<<8) | (1<<10) | (1<<12) | (1<<14);
        ui_gen[1 * 16 + 0] = 0xD | (0xD << 2) | (0xD << 4) | (0xD << 6) | (0xD << 8) | (0xD << 10) | (0xD << 12) | (0xD << 14);
    }
    
    /* Tile 2: Bottom edge */
    {
        for (i = 0; i < 16; i++) ui_gen[2 * 16 + i] = 1 | (1<<2) | (1<<4) | (1<<6) | (1<<8) | (1<<10) | (1<<12) | (1<<14);
        ui_gen[2 * 16 + 15] = 0xD | (0xD << 2) | (0xD << 4) | (0xD << 6) | (0xD << 8) | (0xD << 10) | (0xD << 12) | (0xD << 14);
    }
    
    /* Tile 3: Left edge */
    {
        for (i = 0; i < 16; i++) {
            ui_gen[3 * 16 + i] = 1 | (1<<2) | (1<<4) | (1<<6) | (1<<8) | (1<<10) | (1<<12) | (1<<14);
        }
        /* Set leftmost pixels to border color (palette 13 = border light) */
        for (i = 0; i < 8; i++) {
            /* In 4bpp, first pixel of each row in first halfword */
            ui_gen[3 * 16 + i * 2] = (ui_gen[3 * 16 + i * 2] & 0xFFF0) | 0xD;
        }
    }
    
    /* Tile 4: Right edge */
    {
        for (i = 0; i < 16; i++) {
            ui_gen[4 * 16 + i] = 1 | (1<<2) | (1<<4) | (1<<6) | (1<<8) | (1<<10) | (1<<12) | (1<<14);
        }
        /* Set rightmost pixels to border */
        for (i = 0; i < 8; i++) {
            ui_gen[4 * 16 + i * 2 + 1] = (ui_gen[4 * 16 + i * 2 + 1] & 0x000F) | (0xD << 12);
        }
    }
    
    /* Tiles 5-8: Corner tiles - just use border color fill */
    {
        u16 corner = 0xD | (0xD << 2) | (0xD << 4) | (0xD << 6) | (0xD << 8) | (0xD << 10) | (0xD << 12) | (0xD << 14);
        for (t = 5; t <= 8; t++) {
            for (i = 0; i < 16; i++) ui_gen[t * 16 + i] = corner;
        }
    }
    
    /* Tile 9: Arrow cursor '>' */
    {
        /* Simple arrow pointing right */
        static const u8 arrow[8] = {0x00, 0x10, 0x30, 0x70, 0x30, 0x10, 0x00, 0x00};
        for (int y = 0; y < 8; y++) {
            u16 row[2] = {0, 0};
            for (int x = 0; x < 8; x++) {
                if (arrow[y] & (0x80 >> x)) {
                    row[x/4] |= (0xB << ((x%4) * 4)); /* B = cyan accent */
                }
            }
            ui_gen[9 * 16 + y * 2] = row[0];
            ui_gen[9 * 16 + y * 2 + 1] = row[1];
        }
    }
    
    /* Tile 10: Selection highlight bar */
    {
        u16 hl = 0xC | (0xC << 2) | (0xC << 4) | (0xC << 6) | (0xC << 8) | (0xC << 10) | (0xC << 12) | (0xC << 14);
        for (i = 0; i < 16; i++) ui_gen[10 * 16 + i] = hl;
    }
    
    /* Tiles 11-15: More UI elements */
    /* Tile 11: Lock icon (for gallery) */
    {
        static const u8 lock[8] = {
            0x3C, 0x42, 0x42, 0x42, 0x7E, 0x5A, 0x5A, 0x7E
        };
        for (int y = 0; y < 8; y++) {
            u16 row[2] = {0, 0};
            for (int x = 0; x < 8; x++) {
                if (lock[y] & (0x80 >> x)) {
                    row[x/4] |= (0xD << ((x%4) * 4));
                }
            }
            ui_gen[11 * 16 + y * 2] = row[0];
            ui_gen[11 * 16 + y * 2 + 1] = row[1];
        }
    }
    
    /* Tiles 12-15: Checkmark, X, save icon, settings icon */
    /* Tile 12: Checkmark */
    {
        static const u8 chk[8] = {
            0x00, 0x00, 0x20, 0x40, 0x8E, 0x10, 0x20, 0x00
        };
        for (int y = 0; y < 8; y++) {
            u16 row[2] = {0, 0};
            for (int x = 0; x < 8; x++) {
                if (chk[y] & (0x80 >> x)) {
                    row[x/4] |= (0xB << ((x%4) * 4));
                }
            }
            ui_gen[12 * 16 + y * 2] = row[0];
            ui_gen[12 * 16 + y * 2 + 1] = row[1];
        }
    }
}

void ui_elements_load(void) {
    make_ui_tiles();
    /* Load to BG2 charblock (charblock 2) */
    dma_to_vram(ui_gen, BG_CHARBLOCK(2), sizeof(ui_gen));
}