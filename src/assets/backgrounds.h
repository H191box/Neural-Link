#ifndef BACKGROUNDS_H
#define BACKGROUNDS_H

#include "gba_types.h"

/* Background IDs */
#define BG_ID_CITYSCAPE   0
#define BG_ID_LAB         1
#define BG_ID_MEGACORP    2
#define BG_ID_RESISTANCE  3
#define BG_ID_NETWORK     4
#define BG_ID_COUNT       5

/* Background tile data (tileset) - loaded to BG0 charblock */
extern const u16 bg_tileset[512]; /* Up to 512 tiles for backgrounds */

/* Background tile maps (32x32 each) - loaded to BG0 screenblock */
extern const u16 bg_map_cityscape[1024];
extern const u16 bg_map_lab[1024];
extern const u16 bg_map_megacorp[1024];
extern const u16 bg_map_resistance[1024];
extern const u16 bg_map_network[1024];

/* Load a background tileset to BG0 charblock */
void backgrounds_load_tiles(void);

/* Load a specific background to BG0 screenblock */
void backgrounds_load(int bg_id);

/* Get background name for save/load display */
const char* background_get_name(int bg_id);

#endif /* BACKGROUNDS_H */