#include "gift_sprites.h"
#include "dma.h"
#include "sprite_manager.h"

/*
 * Gift item sprites — generated procedurally.
 * Each is an 8x8 pixel icon representing the gift type.
 * We use simple geometric shapes that are recognizable on GBA.
 *
 * Palette: We'll use OBJ palette 7 for gift items.
 * Color 0 = transparent, Color 1 = outline, Color 2 = fill
 */

/* Generate an 8x8 tile for a gift type */
static void gen_gift_tile(u16 *dst, int gift_type) {
    int x, y;
    u16 tile_buf[16]; /* One 8x8 tile */

    /* Clear tile */
    for (int i = 0; i < 16; i++) tile_buf[i] = 0;

    for (y = 0; y < 8; y++) {
        u16 row_data[2] = {0, 0};
        for (x = 0; x < 8; x++) {
            u8 color = 0; /* transparent by default */

            switch (gift_type) {
                case GIFT_WEAPON: {
                    /* Dagger/sword shape — diagonal line with guard */
                    int dx = x - 4, dy = y - 4;
                    /* Blade (diagonal) */
                    if (y >= 1 && y <= 6) {
                        if (x == 4 - (6 - y)) color = 2; /* Blade edge */
                        if (x == 5 - (6 - y)) color = 1; /* Blade outline */
                    }
                    /* Guard (horizontal bar at y=5) */
                    if (y == 5 && x >= 2 && x <= 6) color = 1;
                    /* Handle (vertical at bottom) */
                    if (y >= 6 && x == 4) color = 2;
                    if (y >= 6 && (x == 3 || x == 5)) color = 1;
                    break;
                }
                case GIFT_LUXURY: {
                    /* Diamond/gem shape */
                    int cx = 4, cy = 4;
                    int dist = (x > cx ? x - cx : cx - x) + (y > cy ? y - cy : cy - y);
                    if (dist <= 3) {
                        color = (dist < 2) ? 2 : 1; /* Fill center, outline edge */
                    }
                    /* Sparkle highlight */
                    if (x == 3 && y == 3) color = 1;
                    break;
                }
                case GIFT_DATA: {
                    /* Chip/disk — rounded rectangle with notch */
                    if (y >= 1 && y <= 6 && x >= 1 && x <= 6) {
                        /* Main body */
                        color = 1;
                        if (y >= 2 && y <= 5 && x >= 2 && x <= 5) {
                            color = 2; /* Inner fill */
                        }
                        /* Notch at bottom-left */
                        if (y == 6 && x <= 3) color = 0;
                        /* Circuit lines */
                        if ((y == 3 || y == 4) && (x == 3 || x == 4)) color = 0;
                    }
                    break;
                }
                case GIFT_BOOK: {
                    /* Open book shape */
                    if (y >= 1 && y <= 6) {
                        /* Left page */
                        if (x >= 1 && x <= 3) {
                            color = 1;
                            if (y >= 2 && y <= 5 && x >= 2) color = 2;
                        }
                        /* Spine */
                        if (x == 4) color = 1;
                        /* Right page */
                        if (x >= 5 && x <= 7) {
                            color = 1;
                            if (y >= 2 && y <= 5 && x <= 6) color = 2;
                        }
                    }
                    break;
                }
                case GIFT_FLOWER: {
                    /* Simple flower — 5 petals around center */
                    int cx = 4, cy = 3;
                    int dx = x - cx, dy = y - cy;
                    int dist = dx * dx + dy * dy;
                    /* Petals (5 blobs) */
                    if (dist <= 6 && dist > 1) {
                        /* Angle-based petals */
                        int angle = 0;
                        if (dx != 0) angle = (dy * 16) / dx;
                        if ((x == 4 && y == 1) || (x == 6 && y == 2) ||
                            (x == 6 && y == 4) || (x == 2 && y == 4) ||
                            (x == 2 && y == 2)) {
                            color = 2; /* Petal */
                        }
                    }
                    /* Center */
                    if (dist <= 1 && dist >= 0) color = 1;
                    /* Stem */
                    if (y >= 5 && y <= 7 && x == 4) color = 1;
                    break;
                }
                case GIFT_MUSIC: {
                    /* Musical note — circle head + stem */
                    /* Note head (circle at 2,5) */
                    int nx = 2, ny = 5;
                    int ndx = x - nx, ndy = y - ny;
                    if (ndx * ndx + ndy * ndy <= 4) color = 2;
                    /* Stem (vertical line up) */
                    if (x == 4 && y >= 1 && y <= 5) color = 1;
                    /* Flag (at top) */
                    if (y == 1 && x >= 4 && x <= 6) color = 2;
                    break;
                }
                case GIFT_FOOD: {
                    /* Cup/plate — rounded U shape */
                    if (y >= 2 && y <= 6) {
                        /* Cup walls */
                        if ((x == 2 || x == 5) && y >= 2 && y <= 5) color = 1;
                        /* Bottom */
                        if (y == 5 && x >= 2 && x <= 5) color = 1;
                        /* Inner area */
                        if (y >= 3 && y <= 4 && x >= 3 && x <= 4) color = 2;
                        /* Handle */
                        if (y == 3 && x == 6) color = 1;
                        if (y == 4 && x == 6) color = 1;
                    }
                    /* Steam */
                    if (y == 1 && (x == 3 || x == 4)) color = 1;
                    break;
                }
                case GIFT_TECH: {
                    /* Gear/circuit shape */
                    int cx = 4, cy = 4;
                    int dist_sq = (x - cx) * (x - cx) + (y - cy) * (y - cy);
                    /* Outer ring */
                    if (dist_sq <= 12 && dist_sq >= 6) color = 1;
                    /* Inner circle */
                    if (dist_sq <= 4) color = 2;
                    /* Gear teeth */
                    if ((x == 0 || x == 7) && y >= 3 && y <= 5) color = 1;
                    if ((y == 0 || y == 7) && x >= 3 && x <= 5) color = 1;
                    break;
                }
            }

            /* Set pixel in row data (4bpp: 4 pixels per u16) */
            row_data[x / 4] |= (color << ((x % 4) * 4));
        }
        tile_buf[y * 2] = row_data[0];
        tile_buf[y * 2 + 1] = row_data[1];
    }

    /* Copy tile to destination */
    for (int i = 0; i < 16; i++) {
        dst[i] = tile_buf[i];
    }
}

void gift_sprites_load(void) {
    /* 8 gift types × 1 tile each = 8 tiles = 128 u16 */
    static u16 gift_tile_data[8 * 16]; /* 8 tiles, 16 halfwords each */
    int type;

    for (type = 0; type < GIFT_SPRITE_COUNT; type++) {
        gen_gift_tile(&gift_tile_data[type * 16], type);
    }

    /* Load to OBJ VRAM after character sprites */
    spr_load_tiles(gift_tile_data, GIFT_SPRITE_TILE_OFFSET, GIFT_SPRITE_COUNT);
}

int gift_sprite_palette(int gift_type) {
    /* All gifts use OBJ palette 7 */
    (void)gift_type;
    return 7;
}