#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "gba_types.h"
#include "gba_header.h"

/* Sprite attribute structure (matches GBA OAM entry) */
typedef struct {
    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 attr3;
} SpriteAttr;

/* OAM entry layout */
typedef struct {
    /* attr0 */
    u16 y : 8;
    u16 shape : 2;
    u16 : 1;
    u16 mode : 2;
    u16 mosaic : 1;
    u16 color_mode : 1;
    u16 : 1;
    /* attr1 */
    u16 x : 9;
    u16 : 3;
    u16 hflip : 1;
    u16 vflip : 1;
    u16 size : 2;
    /* attr2 */
    u16 tile_id : 10;
    u16 priority : 2;
    u16 palette : 4;
} OAMEntry;

/* Sprite shapes and sizes */
#define SPR_SQUARE   0
#define SPR_WIDE     1
#define SPR_TALL     2

/* Sprite sizes (shape, size) -> pixel dimensions */
#define SPR_SIZE_8x8   0
#define SPR_SIZE_16x16 1
#define SPR_SIZE_32x32 2
#define SPR_SIZE_64x64 3

/* Sprite modes */
#define SPR_NORMAL   0
#define SPR_TRANSLUCENT 1
#define SPR_WINDOW   2
#define SPR_HIDDEN   3

/* Max sprites */
#define MAX_SPRITES 64

/* Initialize sprite system */
void spr_init(void);

/* Clear all sprites (hide them) */
void spr_clear_all(void);

/* Update OAM - call after modifying sprite data */
void spr_update(void);

/* Show a sprite at position with tile and palette */
void spr_set(int index, u16 x, u16 y, u16 tile_id, u16 palette,
             u16 shape, u16 size, u16 priority);

/* Hide a sprite */
void spr_hide(int index);

/* Move sprite */
void spr_move(int index, u16 x, u16 y);

/* Change sprite tile */
void spr_set_tile(int index, u16 tile_id);

/* Get sprite pointer for direct manipulation */
OAMEntry* spr_get(int index);

/* Load sprite tile data to OBJ VRAM */
void spr_load_tiles(const u16 *tiles, u16 tile_offset, u16 num_tiles);

/* Load OBJ palette */
void spr_load_palette(const u16 *palette, u16 pal_offset, u16 num_colors);

#endif /* SPRITE_MANAGER_H */