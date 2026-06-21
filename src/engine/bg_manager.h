#ifndef BG_MANAGER_H
#define BG_MANAGER_H

#include "gba_types.h"
#include "gba_header.h"

/* BG Layer assignments for visual novel */
#define BG_SCENE  0   /* Background scene image */
#define BG_CHAR   1   /* Character overlay area */
#define BG_UI     2   /* UI/HUD elements */
#define BG_DIALOG 3   /* Dialog text layer (bottom 48px) */

/* BG configuration modes */
typedef struct {
    u16 cnt;
    u16 priority;
    u16 charblock;
    u16 screenblock;
} BgConfig;

/* Initialize all 4 BG layers for visual novel use */
void bg_init(void);

/* Configure a specific BG layer */
void bg_configure(int layer, u16 priority, u16 charblock, u16 screenblock, u16 size, bool use_8bpp);

/* Set BG scroll offset */
void bg_set_scroll(int layer, s16 x, s16 y);

/* Clear a BG's tile map */
void bg_clear_map(int layer);

/* Clear a BG's character data (charblock) */
void bg_clear_chars(int charblock, int num_tiles);

/* Write a tile to a BG's tile map at (col, row) */
void bg_set_tile(int layer, int col, int row, u16 tile_entry);

/* Write a tile with palette to a BG's tile map */
void bg_set_tile_pal(int layer, int col, int row, u16 tile_id, u16 palette);

/* Fill a BG tile map region with a single tile */
void bg_fill_rect(int layer, int x, int y, int w, int h, u16 tile_entry);

/* Copy tile data to a charblock */
void bg_load_tiles(int charblock, const u16 *tiles, u32 num_tiles);

/* Copy a full tilemap to a screenblock */
void bg_load_map(int screenblock, const u16 *map, u32 size);

/* Enable/disable a BG layer */
void bg_enable(int layer, bool enable);

#endif /* BG_MANAGER_H */