#include "bg_manager.h"
#include "dma.h"

/* VRAM helper: get BG control register for a layer */
static volatile u16* bg_cnt_reg(int layer) {
    switch (layer) {
        case 0: return &REG_BG0CNT;
        case 1: return &REG_BG1CNT;
        case 2: return &REG_BG2CNT;
        case 3: return &REG_BG3CNT;
        default: return &REG_BG0CNT;
    }
}

/* VRAM helper: get scroll register for a layer */
static void bg_scroll(int layer, volatile u16 **hofs, volatile u16 **vofs) {
    switch (layer) {
        case 0: *hofs = &REG_BG0HOFS; *vofs = &REG_BG0VOFS; break;
        case 1: *hofs = &REG_BG1HOFS; *vofs = &REG_BG1VOFS; break;
        case 2: *hofs = &REG_BG2HOFS; *vofs = &REG_BG2VOFS; break;
        case 3: *hofs = &REG_BG3HOFS; *vofs = &REG_BG3VOFS; break;
        default: *hofs = &REG_BG0HOFS; *vofs = &REG_BG0VOFS; break;
    }
}

void bg_init(void) {
    /* 
     * Mode 0 layout for visual novel:
     * BG0 (priority 0): Scene background - charblock 0, screenblock 16 (after tiles)
     * BG1 (priority 1): Character area - charblock 1, screenblock 17
     * BG2 (priority 2): UI elements - charblock 2, screenblock 18
     * BG3 (priority 3): Dialog text - charblock 3, screenblock 19
     * 
     * Screenblocks 16-19 = tile maps at VRAM 0xE000-0xF800
     * Charblocks 0-3 = tile data at VRAM 0x0000-0x8000
     */
    bg_configure(BG_SCENE,  0, 0, 16, BG_SIZE0, FALSE);
    bg_configure(BG_CHAR,   1, 1, 17, BG_SIZE0, FALSE);
    bg_configure(BG_UI,     2, 2, 18, BG_SIZE0, FALSE);
    bg_configure(BG_DIALOG, 3, 3, 19, BG_SIZE0, FALSE);

    /* Clear all tile maps */
    bg_clear_map(BG_SCENE);
    bg_clear_map(BG_CHAR);
    bg_clear_map(BG_UI);
    bg_clear_map(BG_DIALOG);
}

void bg_configure(int layer, u16 priority, u16 charblock, u16 screenblock, u16 size, bool use_8bpp) {
    volatile u16 *cnt = bg_cnt_reg(layer);
    u16 val = BG_PRIORITY(priority) | BG_CHARBASE(charblock) | BG_SCREENBASE(screenblock) | size;
    if (use_8bpp) val |= BG_8BPP;
    *cnt = val;
}

void bg_set_scroll(int layer, s16 x, s16 y) {
    volatile u16 *hofs, *vofs;
    bg_scroll(layer, &hofs, &vofs);
    *hofs = x;
    *vofs = y;
}

void bg_clear_map(int layer) {
    u16 screenblock;
    /* Read screenblock from the control register */
    switch (layer) {
        case 0: screenblock = (REG_BG0CNT >> 8) & 0x1F; break;
        case 1: screenblock = (REG_BG1CNT >> 8) & 0x1F; break;
        case 2: screenblock = (REG_BG2CNT >> 8) & 0x1F; break;
        case 3: screenblock = (REG_BG3CNT >> 8) & 0x1F; break;
        default: screenblock = 16; break;
    }
    u16 *map = BG_SCREENBLOCK(screenblock);
    dma16_fill(0, map, BG_TILE_MAP_SIZE);
}

void bg_clear_chars(int charblock, int num_tiles) {
    u16 *chars = BG_CHARBLOCK(charblock);
    /* Each 4bpp tile is 32 bytes = 16 halfwords */
    dma16_fill(0, chars, num_tiles * 16);
}

void bg_set_tile(int layer, int col, int row, u16 tile_entry) {
    u16 screenblock;
    switch (layer) {
        case 0: screenblock = (REG_BG0CNT >> 8) & 0x1F; break;
        case 1: screenblock = (REG_BG1CNT >> 8) & 0x1F; break;
        case 2: screenblock = (REG_BG2CNT >> 8) & 0x1F; break;
        case 3: screenblock = (REG_BG3CNT >> 8) & 0x1F; break;
        default: screenblock = 16; break;
    }
    u16 *map = BG_SCREENBLOCK(screenblock);
    map[row * TILE_MAP_W + col] = tile_entry;
}

void bg_set_tile_pal(int layer, int col, int row, u16 tile_id, u16 palette) {
    bg_set_tile(layer, col, row, tile_id | (palette << 12));
}

void bg_fill_rect(int layer, int x, int y, int w, int h, u16 tile_entry) {
    u16 screenblock;
    switch (layer) {
        case 0: screenblock = (REG_BG0CNT >> 8) & 0x1F; break;
        case 1: screenblock = (REG_BG1CNT >> 8) & 0x1F; break;
        case 2: screenblock = (REG_BG2CNT >> 8) & 0x1F; break;
        case 3: screenblock = (REG_BG3CNT >> 8) & 0x1F; break;
        default: screenblock = 16; break;
    }
    u16 *map = BG_SCREENBLOCK(screenblock);
    for (int r = y; r < y + h && r < TILE_MAP_H; r++) {
        for (int c = x; c < x + w && c < TILE_MAP_W; c++) {
            map[r * TILE_MAP_W + c] = tile_entry;
        }
    }
}

void bg_load_tiles(int charblock, const u16 *tiles, u32 num_tiles) {
    u16 *dest = BG_CHARBLOCK(charblock);
    dma_to_vram(tiles, dest, num_tiles * 32);
}

void bg_load_map(int screenblock, const u16 *map, u32 size) {
    u16 *dest = BG_SCREENBLOCK(screenblock);
    dma_to_vram(map, dest, size * 2);
}

void bg_enable(int layer, bool enable) {
    if (enable) {
        REG_DISPCNT |= (BG0_ENABLE << layer);
    } else {
        REG_DISPCNT &= ~(BG0_ENABLE << layer);
    }
}