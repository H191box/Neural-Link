#include "backgrounds.h"
#include "bg_manager.h"
#include "dma.h"
#include "gba_header.h"

/*
 * Procedurally generate background tiles for visual novel.
 * Each background uses palette sub-palettes 9-13.
 * Tile indices 0-63 = shared tiles, 64+ = background-specific.
 * 
 * We generate a minimal but effective tileset:
 * - Solid fill tiles (dark, medium, light variants)
 * - Building/window tiles for cityscape
 * - Equipment/screen tiles for lab
 * - Desk/glass tiles for office
 * - Concrete/pipe tiles for resistance
 * - Node/connection tiles for network
 * - Edge/border tiles
 */

/* 
 * Generate tile data procedurally.
 * Each 4bpp tile = 32 bytes = 16 u16 values.
 * Each u16 contains 4 pixels (2 bits each).
 * 
 * We'll define tiles inline as arrays.
 */

/* Helper: create a solid-color tile */
static u16 solid_tile(u16 pal, u16 color) {
    u16 t[16];
    int i;
    u16 packed = (color & 0xF) | ((color & 0xF) << 4) | 
                 ((color & 0xF) << 8) | ((color & 0xF) << 12);
    for (i = 0; i < 16; i++) t[i] = packed;
    /* We return nothing - this is just a helper */
    (void)pal;
    return packed;
}

/*
 * Complete background tileset: 256 tiles
 * Generated at runtime for flexibility.
 * Each tile = 16 u16 values.
 */
u16 bg_tileset[4096]; /* 256 tiles * 16 halfwords */

/*
 * Helper to fill bg_tileset with generated tile data.
 * We generate simple but effective tiles for each background.
 */
static void gen_tile(u16 tile_idx, u16 pal, const u8 pattern[8]) {
    u16 *tile = &bg_tileset[tile_idx * 16];
    int y, x;
    for (y = 0; y < 8; y++) {
        u16 row = 0;
        u8 line = pattern[y];
        for (x = 0; x < 8; x++) {
            u16 c = (line >> (7 - x)) & 1;
            row |= (c << (x * 2));
        }
        /* Each row stored twice (4bpp: 8 pixels = 4 bytes = 2 halfwords per row) */
        tile[y * 2] = row & 0xFFFF;
        tile[y * 2 + 1] = 0; /* Second half = 0 for 4bpp 8-wide */
    }
}

/* Generate tiles at startup */
void backgrounds_load_tiles(void) {
    int i;
    
    /* Clear tileset */
    for (i = 0; i < 4096; i++) bg_tileset[i] = 0;
    
    /* Tile 0: fully transparent (already 0) */
    
    /* Tile 1-3: Solid fills with different palette indices */
    /* These are simple solid-color tiles */
    {
        u16 t;
        /* Solid color 1 */
        t = 1 | (1 << 2) | (1 << 4) | (1 << 6) | (1 << 8) | (1 << 10) | (1 << 12) | (1 << 14);
        for (i = 0; i < 16; i++) bg_tileset[1*16 + i] = t;
        /* Solid color 2 */
        t = 2 | (2 << 2) | (2 << 4) | (2 << 6) | (2 << 8) | (2 << 10) | (2 << 12) | (2 << 14);
        for (i = 0; i < 16; i++) bg_tileset[2*16 + i] = t;
        /* Solid color 3 */
        t = 3 | (3 << 2) | (3 << 4) | (3 << 6) | (3 << 8) | (3 << 10) | (3 << 12) | (3 << 14);
        for (i = 0; i < 16; i++) bg_tileset[3*16 + i] = t;
        /* Solid color 4 */
        t = 4 | (4 << 2) | (4 << 4) | (4 << 6) | (4 << 8) | (4 << 10) | (4 << 12) | (4 << 14);
        for (i = 0; i < 16; i++) bg_tileset[4*16 + i] = t;
        /* Solid color 5 */
        t = 5 | (5 << 2) | (5 << 4) | (5 << 6) | (5 << 8) | (5 << 10) | (5 << 12) | (5 << 14);
        for (i = 0; i < 16; i++) bg_tileset[5*16 + i] = t;
    }
    
    /* Tile 6: Building silhouette (cityscape) */
    {
        u8 bldg[8] = {0xFF, 0x81, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xFF};
        gen_tile(6, 9, bldg);
    }
    /* Tile 7: Window lit */
    {
        u8 win[8] = {0xE7, 0x42, 0x42, 0xE7, 0xE7, 0x42, 0x42, 0xE7};
        gen_tile(7, 9, win);
    }
    /* Tile 8: Window dark */
    {
        u8 win[8] = {0xE7, 0x42, 0x42, 0xE7, 0xE7, 0x42, 0x42, 0xE7};
        gen_tile(8, 9, win);
    }
    /* Tile 9: Neon line horizontal */
    {
        u8 neon[8] = {0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00};
        gen_tile(9, 9, neon);
    }
    /* Tile 10: Neon line vertical */
    {
        u8 neon[8] = {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18};
        gen_tile(10, 9, neon);
    }
    
    /* Lab tiles (11-20) */
    /* Tile 11: Lab wall panel */
    {
        u8 panel[8] = {0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF};
        gen_tile(11, 10, panel);
    }
    /* Tile 12: Screen/monitor */
    {
        u8 scrn[8] = {0x3C, 0x42, 0x99, 0xA5, 0xA5, 0x99, 0x42, 0x3C};
        gen_tile(12, 10, scrn);
    }
    /* Tile 13: Equipment rack */
    {
        u8 rack[8] = {0xFF, 0xAA, 0xAA, 0xFF, 0xFF, 0x55, 0x55, 0xFF};
        gen_tile(13, 10, rack);
    }
    /* Tile 14: Floor tile */
    {
        u8 flr[8] = {0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA};
        gen_tile(14, 10, flr);
    }
    
    /* MegaCorp office tiles (15-20) */
    /* Tile 15: Glass wall */
    {
        u8 glass[8] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};
        gen_tile(15, 11, glass);
    }
    /* Tile 16: Desk */
    {
        u8 desk[8] = {0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00};
        gen_tile(16, 11, desk);
    }
    
    /* Resistance base tiles (17-22) */
    /* Tile 17: Concrete wall */
    {
        u8 conc[8] = {0xFF, 0x99, 0xAA, 0xFF, 0xAA, 0x99, 0xFF, 0x00};
        gen_tile(17, 12, conc);
    }
    /* Tile 18: Pipe horizontal */
    {
        u8 pipe[8] = {0x00, 0x18, 0x3C, 0x7E, 0x7E, 0x3C, 0x18, 0x00};
        gen_tile(18, 12, pipe);
    }
    /* Tile 19: Monitor (resistance) */
    {
        u8 mon[8] = {0x3C, 0x42, 0x81, 0x99, 0x99, 0x81, 0x42, 0x3C};
        gen_tile(19, 12, mon);
    }
    
    /* Neural network tiles (20-30) */
    /* Tile 20: Network node */
    {
        u8 node[8] = {0x00, 0x3C, 0x7E, 0x7E, 0x7E, 0x7E, 0x3C, 0x00};
        gen_tile(20, 13, node);
    }
    /* Tile 21: Network connection (diagonal) */
    {
        u8 conn[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
        gen_tile(21, 13, conn);
    }
    /* Tile 22: Network connection (anti-diagonal) */
    {
        u8 conn[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
        gen_tile(22, 13, conn);
    }
    /* Tile 23: Grid line H */
    {
        u8 grid[8] = {0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00};
        gen_tile(23, 13, grid);
    }
    /* Tile 24: Grid line V */
    {
        u8 grid[8] = {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18};
        gen_tile(24, 13, grid);
    }
    /* Tile 25: Data burst */
    {
        u8 data[8] = {0xAA, 0x55, 0xAA, 0xFF, 0xFF, 0xAA, 0x55, 0xAA};
        gen_tile(25, 13, data);
    }
    
    /* UI tiles (for BG2) - we use a separate area */
    /* These are loaded to charblock 2 */
    
    /* Upload to BG0 charblock (charblock 0) */
    dma_to_vram(bg_tileset, BG_CHARBLOCK(0), sizeof(bg_tileset));
}

/*
 * Generate tile maps procedurally for each background.
 * Each map is 32x32 tiles = 1024 entries.
 * Each entry: tile_id | (palette << 12) | (hflip << 11) | (vflip << 10)
 */

/* Cityscape background: dark sky with buildings and neon lights */
static u16 map_city[1024];
const u16 bg_map_cityscape[1024]; /* Extern declaration match */

/* Lab background: clean walls, equipment, monitors */
static u16 map_lab[1024];
const u16 bg_map_lab[1024];

/* MegaCorp office: glass walls, desks, clean corporate */
static u16 map_office[1024];
const u16 bg_map_megacorp[1024];

/* Resistance base: concrete, pipes, monitors */
static u16 map_resist[1024];
const u16 bg_map_resistance[1024];

/* Neural network: abstract digital space */
static u16 map_net[1024];
const u16 bg_map_network[1024];

/* Palette offset for backgrounds (sub-palette 9 = pal index 9) */
#define BG_PAL(pal_idx) ((pal_idx) << 12)

static void generate_cityscape(void) {
    int x, y;
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 30; x++) {
            if (y < 8) {
                /* Sky - dark gradient */
                map_city[y * 32 + x] = (y < 4) ? 1 | BG_PAL(9) : 2 | BG_PAL(9);
            } else if (y < 12) {
                /* Buildings upper - various heights */
                int building_x = x % 6;
                if (building_x == 0 || building_x == 5) {
                    map_city[y * 32 + x] = 6 | BG_PAL(9); /* Building silhouette */
                } else if (y == 8 && (x % 3 == 1)) {
                    map_city[y * 32 + x] = 7 | BG_PAL(9); /* Lit window */
                } else {
                    map_city[y * 32 + x] = 6 | BG_PAL(9);
                }
            } else if (y < 16) {
                /* Buildings lower */
                if (x % 5 == 0) {
                    map_city[y * 32 + x] = 9 | BG_PAL(9); /* Neon line */
                } else {
                    map_city[y * 32 + x] = 3 | BG_PAL(9);
                }
            } else {
                /* Ground */
                map_city[y * 32 + x] = (y < 18) ? 14 | BG_PAL(10) : 14 | BG_PAL(10);
            }
        }
        /* Right edge: sky/side building */
        map_city[y * 32 + 30] = 2 | BG_PAL(9);
        map_city[y * 32 + 31] = 1 | BG_PAL(9);
    }
}

static void generate_lab(void) {
    int x, y;
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 30; x++) {
            if (y < 4) {
                /* Ceiling */
                map_lab[y * 32 + x] = 11 | BG_PAL(10);
            } else if (y < 14) {
                /* Wall area */
                if (x == 0 || x == 29 || x == 7 || x == 22) {
                    map_lab[y * 32 + x] = 11 | BG_PAL(10); /* Wall panel */
                } else if (x > 8 && x < 12 && y > 5 && y < 10) {
                    map_lab[y * 32 + x] = 12 | BG_PAL(10); /* Monitor */
                } else if (x > 15 && x < 20 && y > 4 && y < 13) {
                    map_lab[y * 32 + x] = 13 | BG_PAL(10); /* Equipment */
                } else {
                    map_lab[y * 32 + x] = 2 | BG_PAL(10); /* Wall */
                }
            } else {
                /* Floor */
                map_lab[y * 32 + x] = 14 | BG_PAL(10);
            }
        }
        map_lab[y * 32 + 30] = 11 | BG_PAL(10);
        map_lab[y * 32 + 31] = 11 | BG_PAL(10);
    }
}

static void generate_megacorp(void) {
    int x, y;
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 30; x++) {
            if (y < 3) {
                /* Ceiling */
                map_office[y * 32 + x] = 3 | BG_PAL(11);
            } else if (y < 12) {
                /* Wall with glass */
                if (x < 5 || x > 24) {
                    map_office[y * 32 + x] = 2 | BG_PAL(11); /* Side wall */
                } else if (x == 5 || x == 24) {
                    map_office[y * 32 + x] = 15 | BG_PAL(11); /* Glass edge */
                } else {
                    map_office[y * 32 + x] = 15 | BG_PAL(11); /* Glass wall */
                }
            } else {
                /* Desk area */
                if (y >= 14 && y < 16 && x > 3 && x < 26) {
                    map_office[y * 32 + x] = 16 | BG_PAL(11); /* Desk */
                } else {
                    map_office[y * 32 + x] = 4 | BG_PAL(11); /* Floor */
                }
            }
        }
        map_office[y * 32 + 30] = 2 | BG_PAL(11);
        map_office[y * 32 + 31] = 3 | BG_PAL(11);
    }
}

static void generate_resistance(void) {
    int x, y;
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 30; x++) {
            if (y < 3) {
                /* Low ceiling (bunker) */
                map_resist[y * 32 + x] = 17 | BG_PAL(12);
            } else if (y < 13) {
                /* Concrete walls */
                if (x == 0 || x == 29) {
                    map_resist[y * 32 + x] = 17 | BG_PAL(12);
                } else if (y == 6) {
                    map_resist[y * 32 + x] = 18 | BG_PAL(12); /* Pipe */
                } else if (x > 8 && x < 13 && y > 7 && y < 11) {
                    map_resist[y * 32 + x] = 19 | BG_PAL(12); /* Monitor */
                } else {
                    map_resist[y * 32 + x] = ((x + y) % 3) + 2 | BG_PAL(12);
                }
            } else {
                /* Rough floor */
                map_resist[y * 32 + x] = ((x * 7 + y * 13) % 4) + 2 | BG_PAL(12);
            }
        }
        map_resist[y * 32 + 30] = 17 | BG_PAL(12);
        map_resist[y * 32 + 31] = 1 | BG_PAL(12);
    }
}

static void generate_network(void) {
    int x, y;
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 30; x++) {
            /* Dark background with grid */
            if (y % 4 == 0) {
                map_net[y * 32 + x] = 23 | BG_PAL(13); /* H grid */
            } else if (x % 4 == 0) {
                map_net[y * 32 + x] = 24 | BG_PAL(13); /* V grid */
            } else {
                map_net[y * 32 + x] = 1 | BG_PAL(13); /* Dark void */
            }
            /* Place nodes at regular intervals */
            if (x % 6 == 3 && y % 6 == 3 && y > 2) {
                map_net[y * 32 + x] = 20 | BG_PAL(13); /* Node */
            }
            /* Some connections */
            if (y % 6 == 4 && x % 6 == 4 && y > 2) {
                map_net[y * 32 + x] = 21 | BG_PAL(13); /* Connection */
            }
            /* Data bursts randomly */
            if ((x * 17 + y * 31) % 23 == 0) {
                map_net[y * 32 + x] = 25 | BG_PAL(13);
            }
        }
        map_net[y * 32 + 30] = 1 | BG_PAL(13);
        map_net[y * 32 + 31] = 1 | BG_PAL(13);
    }
}

void backgrounds_load(int bg_id) {
    /* Regenerate maps each time (cheap, ensures fresh data) */
    switch (bg_id) {
        case BG_ID_CITYSCAPE:
            generate_cityscape();
            bg_load_map(16, map_city, 1024);
            break;
        case BG_ID_LAB:
            generate_lab();
            bg_load_map(16, map_lab, 1024);
            break;
        case BG_ID_MEGACORP:
            generate_megacorp();
            bg_load_map(16, map_office, 1024);
            break;
        case BG_ID_RESISTANCE:
            generate_resistance();
            bg_load_map(16, map_resist, 1024);
            break;
        case BG_ID_NETWORK:
            generate_network();
            bg_load_map(16, map_net, 1024);
            break;
    }
}

const char* background_get_name(int bg_id) {
    switch (bg_id) {
        case BG_ID_CITYSCAPE:  return "City";
        case BG_ID_LAB:        return "Lab";
        case BG_ID_MEGACORP:   return "Office";
        case BG_ID_RESISTANCE: return "Base";
        case BG_ID_NETWORK:    return "Network";
        default:               return "Unknown";
    }
}