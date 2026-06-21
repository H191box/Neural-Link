#include "characters.h"
#include "gba_header.h"
#include "dma.h"
#include "sprite_manager.h"

/* 
 * Character portrait sprites generated procedurally.
 * Each portrait is 32x32 pixels (4 tiles wide, 4 tiles tall = 16 tiles).
 * We generate simplified pixel-art faces.
 * 
 * Stored as 4bpp tile data, loaded to OBJ VRAM.
 * Character portraits at: tile offset = char_id * 32 tiles (for 2 expressions)
 */

static const char* char_names[] = {
    "Kira", "Nova", "Chen", "ARIA", "Vex"
};

/* Generate a character face as tile data */
static void gen_face(u16 *dst, int palette, int face_type) {
    /* 
     * Generate a 32x32 face (16 tiles of 32 bytes each)
     * Simplified: head oval, eyes, mouth, hair
     * face_type: 0=neutral, 1=alternate expression
     */
    int x, y;
    u16 tile_buf[16]; /* One 8x8 tile */
    
    for (int ty = 0; ty < 4; ty++) { /* 4 tile rows */
        for (int tx = 0; tx < 4; tx++) { /* 4 tile cols */
            /* Clear tile */
            for (int i = 0; i < 16; i++) tile_buf[i] = 0;
            
            /* Draw pixels within this 8x8 tile */
            for (y = 0; y < 8; y++) {
                u16 row_data[2] = {0, 0};
                for (x = 0; x < 8; x++) {
                    int px = tx * 8 + x; /* Global pixel x */
                    int py = ty * 8 + y; /* Global pixel y */
                    
                    /* Center of face at (16, 14), radius ~12 */
                    int dx = px - 16;
                    int dy = py - 14;
                    int dist_sq = dx * dx + dy * dy;
                    int face_r = 11;
                    
                    u8 color = 0; /* transparent */
                    
                    /* Head shape (oval) */
                    if (dist_sq < face_r * face_r && dist_sq > (face_r - 2) * (face_r - 2)) {
                        color = 6; /* hair/outline */
                    } else if (dist_sq < (face_r - 2) * (face_r - 2)) {
                        /* Inside head - skin */
                        color = 1; /* skin */
                        
                        /* Hair on top */
                        if (py < 8 && dy < -4) {
                            color = 4; /* hair */
                        }
                        
                        /* Eyes at y=13, x=11 and x=21 */
                        int eye_y = 13, eye_x1 = 11, eye_x2 = 21;
                        if (py == eye_y && (px == eye_x1 || px == eye_x2)) {
                            color = 10; /* eye white */
                        }
                        if (py == eye_y && (px == eye_x1 + 1 || px == eye_x2 + 1)) {
                            color = 11; /* eye color */
                        }
                        if (py == eye_y && (px == eye_x1 + 2 || px == eye_x2 + 2)) {
                            color = 12; /* eye dark */
                        }
                        
                        /* Eyebrows */
                        if (py == eye_y - 2 && px >= eye_x1 - 1 && px <= eye_x1 + 3) {
                            color = 4;
                        }
                        if (py == eye_y - 2 && px >= eye_x2 - 1 && px <= eye_x2 + 3) {
                            color = 4;
                        }
                        
                        /* Nose */
                        if (px == 16 && py == 17) {
                            color = 2; /* skin shadow */
                        }
                        
                        /* Mouth */
                        if (face_type == 0) {
                            /* Neutral mouth */
                            if (py == 20 && px >= 13 && px <= 19) {
                                color = 3; /* darker skin shadow */
                            }
                        } else {
                            /* Alternate expression (slight smile) */
                            if (py == 20 && px >= 13 && px <= 19) {
                                color = 3;
                            }
                            if (py == 19 && (px == 13 || px == 19)) {
                                color = 3;
                            }
                        }
                        
                        /* Neck/collar */
                        if (py > 23 && px > 12 && px < 20) {
                            color = 8; /* coat/shirt */
                        }
                    }
                    
                    /* Set pixel in row data */
                    row_data[x / 4] |= (color << ((x % 4) * 4));
                }
                tile_buf[y * 2] = row_data[0];
                tile_buf[y * 2 + 1] = row_data[1];
            }
            
            /* Copy tile to destination */
            int tile_offset = (ty * 4 + tx) * 16;
            for (int i = 0; i < 16; i++) {
                dst[tile_offset + i] = tile_buf[i];
            }
        }
    }
}

/* Static buffer for generated sprites */
/* 5 chars * 2 expressions * 16 tiles * 16 halfwords = 2560 u16 */
static u16 char_sprite_data[2560];

void characters_load_sprites(void) {
    int ch, expr;
    
    for (ch = 0; ch < CHAR_COUNT; ch++) {
        for (expr = 0; expr < 2; expr++) {
            int offset = (ch * 2 + expr) * 256; /* 16 tiles * 16 u16 */
            gen_face(&char_sprite_data[offset], ch + 2, expr);
        }
    }
    
    /* Load all character sprites to OBJ VRAM starting at tile 0 */
    spr_load_tiles(char_sprite_data, 0, CHAR_COUNT * 2 * 16);
}

const char* character_get_name(int char_id) {
    if (char_id >= 0 && char_id < CHAR_COUNT) return char_names[char_id];
    return "???";
}

int character_get_palette(int char_id) {
    /* Palette 2 = Kai, 3 = Nova, 4 = Chen, 5 = ARIA, 6 = Vex */
    return char_id + 2;
}