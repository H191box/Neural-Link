#ifndef CHARACTERS_ASSET_H
#define CHARACTERS_ASSET_H

#include "gba_types.h"

/* Character IDs (asset sprite indices, kept for compat) */
#define CHAR_KAI    0  /* Reused for Kira (female protagonist) */
#define CHAR_NOVA   1
#define CHAR_CHEN   2
#define CHAR_ARIA   3
#define CHAR_VEX    4
#define CHAR_COUNT  5

/* Expression IDs */
#define EXPR_NORMAL   0
#define EXPR_HAPPY    1
#define EXPR_ANGRY    2
#define EXPR_SAD      3
#define EXPR_SURPRISED 4

/* Character sprite data: each character has expressions as 32x32 4bpp sprites */
/* Each 32x32 sprite = 16 tiles = 512 bytes = 256 u16 */
/* We store 2 expressions per character to keep ROM size manageable */

/* Load character sprite tiles to OBJ VRAM */
void characters_load_sprites(void);

/* Get character name */
const char* character_get_name(int char_id);

/* Get character OBJ palette index */
int character_get_palette(int char_id);

#endif /* CHARACTERS_ASSET_H */