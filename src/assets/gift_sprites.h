#ifndef GIFT_SPRITES_H
#define GIFT_SPRITES_H

#include "gba_types.h"

/* Gift type sprite IDs (one sprite per type) */
#define GIFT_SPRITE_WEAPON  0
#define GIFT_SPRITE_LUXURY  1
#define GIFT_SPRITE_DATA    2
#define GIFT_SPRITE_BOOK    3
#define GIFT_SPRITE_FLOWER  4
#define GIFT_SPRITE_MUSIC   5
#define GIFT_SPRITE_FOOD    6
#define GIFT_SPRITE_TECH    7
#define GIFT_SPRITE_COUNT   8

/* Each gift sprite is 8x8 pixels = 1 tile = 32 bytes */
#define GIFT_SPRITE_TILE_OFFSET 160  /* Load after character sprites (5*2*16=160 tiles) */

/* Load all gift sprites to OBJ VRAM */
void gift_sprites_load(void);

/* Get the OBJ palette for a gift type */
int gift_sprite_palette(int gift_type);

#endif /* GIFT_SPRITES_H */