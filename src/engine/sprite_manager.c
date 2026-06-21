#include "sprite_manager.h"
#include "dma.h"

/* OAM buffer in EWRAM (aligned) */
static OAMEntry oam_buffer[MAX_SPRITES] __attribute__((aligned(32)));

void spr_init(void) {
    spr_clear_all();
    spr_update();
}

void spr_clear_all(void) {
    int i;
    for (i = 0; i < MAX_SPRITES; i++) {
        oam_buffer[i].y = 160; /* Off-screen */
        oam_buffer[i].x = 240;
        oam_buffer[i].attr2 = 0;
        oam_buffer[i].attr0 = 0;
        oam_buffer[i].shape = 0;
        oam_buffer[i].mode = SPR_HIDDEN;
        oam_buffer[i].mosaic = 0;
        oam_buffer[i].color_mode = 0;
        oam_buffer[i].hflip = 0;
        oam_buffer[i].vflip = 0;
        oam_buffer[i].size = 0;
        oam_buffer[i].tile_id = 0;
        oam_buffer[i].priority = 0;
        oam_buffer[i].palette = 0;
    }
}

void spr_update(void) {
    dma_to_oam(oam_buffer, (void*)OAM_BASE, sizeof(oam_buffer));
}

void spr_set(int index, u16 x, u16 y, u16 tile_id, u16 palette,
             u16 shape, u16 size, u16 priority) {
    if (index >= MAX_SPRITES) return;
    oam_buffer[index].y = y;
    oam_buffer[index].x = x;
    oam_buffer[index].tile_id = tile_id;
    oam_buffer[index].palette = palette;
    oam_buffer[index].shape = shape;
    oam_buffer[index].size = size;
    oam_buffer[index].priority = priority;
    oam_buffer[index].mode = SPR_NORMAL;
    oam_buffer[index].color_mode = 0; /* 4bpp */
    oam_buffer[index].mosaic = 0;
    oam_buffer[index].hflip = 0;
    oam_buffer[index].vflip = 0;
}

void spr_hide(int index) {
    if (index >= MAX_SPRITES) return;
    oam_buffer[index].y = 160;
    oam_buffer[index].mode = SPR_HIDDEN;
}

void spr_move(int index, u16 x, u16 y) {
    if (index >= MAX_SPRITES) return;
    oam_buffer[index].x = x;
    oam_buffer[index].y = y;
}

void spr_set_tile(int index, u16 tile_id) {
    if (index >= MAX_SPRITES) return;
    oam_buffer[index].tile_id = tile_id;
}

OAMEntry* spr_get(int index) {
    if (index >= MAX_SPRITES) return NULL;
    return &oam_buffer[index];
}

void spr_load_tiles(const u16 *tiles, u16 tile_offset, u16 num_tiles) {
    u16 *dest = (u16*)(OBJ_BASE + tile_offset * 32);
    dma_to_vram(tiles, dest, num_tiles * 32);
}

void spr_load_palette(const u16 *palette, u16 pal_offset, u16 num_colors) {
    u16 *dest = PALETTE_OBJ + pal_offset * 16;
    dma_to_palette(palette, dest, num_colors * 2);
}