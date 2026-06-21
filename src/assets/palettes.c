#include "palettes.h"
#include "gba_header.h"
#include "dma.h"

/*
 * Background palette layout (256 entries):
 * 
 * Sub-palette 0 (0-15): Scene BG colors (cityscape, lab, etc.)
 * Sub-palette 1 (16-31): UI/Dialog box
 * Sub-palette 2 (32-47): Text color WHITE
 * Sub-palette 3 (48-63): Text color YELLOW
 * Sub-palette 4 (64-79): Text color CYAN
 * Sub-palette 5 (80-95): Text color RED
 * Sub-palette 6 (96-111): Text color GREEN
 * Sub-palette 7 (112-127): Text color GRAY
 * Sub-palette 8 (128-143): Title/menu colors
 * Sub-palette 9 (144-159): Cityscape BG
 * Sub-palette 10 (160-175): Neural Lab BG
 * Sub-palette 11 (176-191): MegaCorp BG
 * Sub-palette 12 (192-207): Resistance BG
 * Sub-palette 13 (208-223): Neural Network BG
 * Sub-palette 14-15: Spare
 */
const u16 bg_palette[256] = {
    /* Sub-palette 0: Base scene colors */
    RGB8(0,0,0),       /* 0: transparent/black */
    RGB8(20,20,40),    /* 1: very dark blue */
    RGB8(40,40,80),    /* 2: dark blue */
    RGB8(60,60,120),   /* 3: medium blue */
    RGB8(80,80,160),   /* 4: blue */
    RGB8(100,100,200), /* 5: light blue */
    RGB8(120,120,220), /* 6: pale blue */
    RGB8(140,140,240), /* 7: very pale blue */
    RGB8(0,40,40),     /* 8: very dark teal */
    RGB8(0,80,80),     /* 9: dark teal */
    RGB8(0,120,120),   /* 10: teal */
    RGB8(0,160,160),   /* 11: light teal */
    RGB8(0,200,200),   /* 12: bright teal */
    RGB8(40,0,60),     /* 13: dark purple */
    RGB8(80,0,120),    /* 14: purple */
    RGB8(120,0,180),   /* 15: bright purple */
    
    /* Sub-palette 1: UI/Dialog box (indices 16-31) */
    RGB8(0,0,0),       /* 16: BG - black */
    RGB8(10,12,20),    /* 17: dark panel BG */
    RGB8(15,18,30),    /* 18: panel BG */
    RGB8(20,25,40),    /* 19: panel BG light */
    RGB8(0,180,200),   /* 20: cyan accent */
    RGB8(0,220,240),   /* 21: bright cyan */
    RGB8(0,140,160),   /* 22: dim cyan */
    RGB8(0,100,120),   /* 23: very dim cyan */
    RGB8(180,180,200), /* 24: light gray text */
    RGB8(200,200,220), /* 25: white text */
    RGB8(255,255,255), /* 26: pure white */
    RGB8(255,220,50),  /* 27: yellow highlight */
    RGB8(200,170,40),  /* 28: dim yellow */
    RGB8(80,80,100),   /* 29: border color */
    RGB8(120,120,150), /* 30: border light */
    RGB8(50,50,70),    /* 31: border dark */
    
    /* Sub-palette 2: Text WHITE (indices 32-47) - only index 1 matters */
    RGB8(0,0,0),       /* 32: transparent */
    RGB8(220,220,240), /* 33: white text */
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0),
    
    /* Sub-palette 3: Text YELLOW (indices 48-63) */
    RGB8(0,0,0),       /* 48: transparent */
    RGB8(255,220,50),  /* 49: yellow text */
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0),
    
    /* Sub-palette 4: Text CYAN (indices 64-79) */
    RGB8(0,0,0),       /* 64: transparent */
    RGB8(0,200,220),   /* 65: cyan text */
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0),
    
    /* Sub-palette 5: Text RED (indices 80-95) */
    RGB8(0,0,0),       /* 80: transparent */
    RGB8(220,50,50),   /* 81: red text */
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0),
    
    /* Sub-palette 6: Text GREEN (indices 96-111) */
    RGB8(0,0,0),       /* 96: transparent */
    RGB8(50,220,80),   /* 97: green text */
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0),
    
    /* Sub-palette 7: Text GRAY (indices 112-127) */
    RGB8(0,0,0),       /* 112: transparent */
    RGB8(150,150,170), /* 113: gray text */
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0),
    
    /* Sub-palette 8: Title/menu (indices 128-143) */
    RGB8(0,0,0),       /* 128: black */
    RGB8(10,10,25),    /* 129: dark BG */
    RGB8(0,60,80),     /* 130: dark cyan */
    RGB8(0,120,160),   /* 131: medium cyan */
    RGB8(0,180,220),   /* 132: cyan */
    RGB8(0,220,255),   /* 133: bright cyan */
    RGB8(100,255,255), /* 134: white-cyan */
    RGB8(200,200,220), /* 135: light */
    RGB8(255,255,255), /* 136: white */
    RGB8(255,200,0),   /* 137: gold */
    RGB8(200,150,0),   /* 138: dim gold */
    RGB8(150,100,0),   /* 139: dark gold */
    RGB8(80,60,100),   /* 140: purple accent */
    RGB8(120,80,160),  /* 141: medium purple */
    RGB8(60,60,80),    /* 142: dark gray */
    RGB8(40,40,60),    /* 143: very dark */
    
    /* Sub-palette 9: Cityscape (indices 144-159) */
    RGB8(5,5,15),      /* 144: night sky */
    RGB8(10,10,30),    /* 145: dark sky */
    RGB8(20,20,50),    /* 146: sky gradient */
    RGB8(40,30,60),    /* 147: purple sky */
    RGB8(30,30,50),    /* 148: building dark */
    RGB8(50,50,70),    /* 149: building mid */
    RGB8(70,70,90),    /* 150: building light */
    RGB8(90,90,110),   /* 151: building highlight */
    RGB8(0,200,255),   /* 152: neon cyan */
    RGB8(255,100,50),  /* 153: neon orange */
    RGB8(255,200,0),   /* 154: neon yellow */
    RGB8(200,50,255),  /* 155: neon purple */
    RGB8(50,255,100),  /* 156: neon green */
    RGB8(20,20,35),    /* 157: ground dark */
    RGB8(40,40,55),    /* 158: ground */
    RGB8(60,60,80),    /* 159: ground light */
    
    /* Sub-palette 10: Neural Lab (indices 160-175) */
    RGB8(15,15,20),    /* 160: lab BG dark */
    RGB8(25,25,35),    /* 161: lab wall */
    RGB8(35,40,50),    /* 162: lab wall mid */
    RGB8(50,55,70),    /* 163: lab wall light */
    RGB8(0,150,180),   /* 164: screen glow */
    RGB8(0,200,240),   /* 165: screen bright */
    RGB8(100,120,140), /* 166: equipment */
    RGB8(130,140,160), /* 167: equipment light */
    RGB8(60,70,80),    /* 168: metal dark */
    RGB8(80,90,100),   /* 169: metal */
    RGB8(200,200,220), /* 170: white surface */
    RGB8(0,255,200),   /* 171: green indicator */
    RGB8(255,50,50),   /* 172: red warning */
    RGB8(255,200,0),   /* 173: amber */
    RGB8(40,40,50),    /* 174: floor */
    RGB8(60,60,75),    /* 175: floor light */
    
    /* Sub-palette 11: MegaCorp Office (indices 176-191) */
    RGB8(20,20,30),    /* 176: dark */
    RGB8(40,35,50),    /* 177: wall dark */
    RGB8(60,55,70),    /* 178: wall */
    RGB8(80,75,90),    /* 179: wall light */
    RGB8(120,115,130), /* 180: surface */
    RGB8(160,155,170), /* 181: surface light */
    RGB8(200,200,210), /* 182: bright surface */
    RGB8(220,220,230), /* 183: white surface */
    RGB8(0,100,120),   /* 184: corporate teal */
    RGB8(0,150,180),   /* 185: corporate teal bright */
    RGB8(180,180,200), /* 186: glass */
    RGB8(220,220,240), /* 187: glass light */
    RGB8(100,100,120), /* 188: desk */
    RGB8(70,70,85),    /* 189: desk dark */
    RGB8(50,50,65),    /* 190: trim */
    RGB8(30,30,45),    /* 191: trim dark */
    
    /* Sub-palette 12: Resistance Base (indices 192-207) */
    RGB8(15,15,10),    /* 192: bunker dark */
    RGB8(30,28,25),    /* 193: concrete dark */
    RGB8(50,48,45),    /* 194: concrete */
    RGB8(70,68,65),    /* 195: concrete light */
    RGB8(90,88,85),    /* 196: concrete bright */
    RGB8(110,105,100), /* 197: worn surface */
    RGB8(0,180,100),   /* 198: rebel green */
    RGB8(0,220,130),   /* 199: rebel green bright */
    RGB8(200,150,50),  /* 200: amber light */
    RGB8(255,200,80),  /* 201: warm light */
    RGB8(0,100,80),    /* 202: dim green */
    RGB8(40,40,35),    /* 203: pipe dark */
    RGB8(60,60,55),    /* 204: pipe */
    RGB8(80,80,75),    /* 205: pipe light */
    RGB8(120,100,80),  /* 206: rust */
    RGB8(25,25,20),    /* 207: shadow */
    
    /* Sub-palette 13: Neural Network (indices 208-223) */
    RGB8(0,0,10),      /* 208: void dark */
    RGB8(5,5,25),      /* 209: void */
    RGB8(10,10,40),    /* 210: void light */
    RGB8(0,100,150),   /* 211: node dim */
    RGB8(0,180,255),   /* 212: node */
    RGB8(100,220,255), /* 213: node bright */
    RGB8(200,240,255), /* 214: node core */
    RGB8(0,60,100),    /* 215: connection */
    RGB8(0,100,160),   /* 216: connection bright */
    RGB8(80,0,160),    /* 217: data purple */
    RGB8(160,0,255),   /* 218: data bright */
    RGB8(200,100,255), /* 219: data glow */
    RGB8(0,255,200),   /* 220: success green */
    RGB8(255,50,50),   /* 221: danger red */
    RGB8(255,255,100), /* 222: warning yellow */
    RGB8(255,255,255), /* 223: white flash */
    
    /* Sub-palette 14: Spare (224-239) */
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    
    /* Sub-palette 15: Spare (240-255) */
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
};

/*
 * Object palette (256 entries):
 * Sub-palette 0: Cursor/sprites
 * Sub-palette 1: UI sprites (arrows, icons)
 * Sub-palette 2-15: Character sprites
 */
const u16 obj_palette[256] = {
    /* Sub-palette 0: General sprites (0-15) */
    RGB8(0,0,0),       /* 0: transparent */
    RGB8(220,220,240), /* 1: white */
    RGB8(0,200,220),   /* 2: cyan */
    RGB8(0,255,200),   /* 3: green */
    RGB8(255,220,50),  /* 4: yellow */
    RGB8(255,100,50),  /* 5: orange */
    RGB8(220,50,50),   /* 6: red */
    RGB8(200,50,255),  /* 7: purple */
    RGB8(100,100,120), /* 8: dark gray */
    RGB8(150,150,170), /* 9: mid gray */
    RGB8(180,180,200), /* 10: light gray */
    RGB8(40,40,60),    /* 11: very dark */
    RGB8(80,80,100),   /* 12: dim */
    RGB8(0,150,180),   /* 13: dim cyan */
    RGB8(200,200,220), /* 14: off-white */
    RGB8(255,255,255), /* 15: pure white */
    
    /* Sub-palette 1: UI icons (16-31) */
    RGB8(0,0,0),       /* 16: transparent */
    RGB8(255,220,50),  /* 17: gold */
    RGB8(200,170,40),  /* 18: dim gold */
    RGB8(255,255,255), /* 19: white */
    RGB8(0,200,220),   /* 20: cyan */
    RGB8(0,150,180),   /* 21: dim cyan */
    RGB8(50,220,80),   /* 22: green */
    RGB8(220,50,50),   /* 23: red */
    RGB8(100,100,120), /* 24: gray */
    RGB8(60,60,80),    /* 25: dark */
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    
    /* Sub-palettes 2-15: Character sprite colors */
    /* Sub-palette 2: Kai (player) - blue tones (32-47) */
    RGB8(0,0,0),
    RGB8(220,200,180), /* skin */
    RGB8(180,160,140), /* skin shadow */
    RGB8(60,60,100),   /* hair dark */
    RGB8(80,80,130),   /* hair */
    RGB8(100,100,160), /* hair highlight */
    RGB8(30,50,100),   /* coat dark */
    RGB8(40,70,140),   /* coat */
    RGB8(60,90,170),   /* coat light */
    RGB8(80,110,200),  /* coat highlight */
    RGB8(200,200,220), /* eye white */
    RGB8(0,150,200),   /* eye blue */
    RGB8(0,100,150),   /* eye dark */
    RGB8(180,180,200), /* shirt */
    RGB8(40,40,60),    /* pants */
    RGB8(30,30,50),    /* shoes */
    
    /* Sub-palette 3: Nova - green/red tones (48-63) */
    RGB8(0,0,0),
    RGB8(200,170,150), /* skin */
    RGB8(160,130,110), /* skin shadow */
    RGB8(20,80,40),    /* hair dark */
    RGB8(30,120,60),   /* hair */
    RGB8(50,160,80),   /* hair highlight */
    RGB8(60,20,20),    /* jacket dark */
    RGB8(100,30,30),   /* jacket */
    RGB8(140,50,50),   /* jacket light */
    RGB8(200,200,210), /* eye white */
    RGB8(0,200,100),   /* eye green */
    RGB8(0,150,70),    /* eye dark */
    RGB8(80,20,20),    /* pants */
    RGB8(60,60,60),    /* boots */
    RGB8(0,0,0), RGB8(0,0,0),
    
    /* Sub-palette 4: Director Chen - gray/white (64-79) */
    RGB8(0,0,0),
    RGB8(220,200,180), /* skin */
    RGB8(180,160,140), /* skin shadow */
    RGB8(80,80,80),    /* hair (gray) */
    RGB8(110,110,110), /* hair */
    RGB8(140,140,140), /* hair */
    RGB8(200,200,210), /* suit white */
    RGB8(170,170,185), /* suit */
    RGB8(140,140,155), /* suit shadow */
    RGB8(220,220,230), /* shirt */
    RGB8(200,200,220), /* eye white */
    RGB8(150,50,50),   /* eye red */
    RGB8(100,30,30),   /* eye dark */
    RGB8(50,50,60),    /* pants */
    RGB8(40,40,50),    /* shoes */
    RGB8(0,200,220),   /* tie accent */
    
    /* Sub-palette 5: ARIA - cyan/white (80-95) */
    RGB8(0,0,0),
    RGB8(180,220,255), /* skin (pale) */
    RGB8(140,180,220), /* skin shadow */
    RGB8(0,180,220),   /* hair cyan */
    RGB8(0,200,240),   /* hair bright */
    RGB8(100,220,255), /* hair highlight */
    RGB8(40,80,120),   /* body dark */
    RGB8(60,100,140),  /* body */
    RGB8(80,120,160),  /* body light */
    RGB8(200,240,255), /* eye white */
    RGB8(0,255,200),   /* eye */
    RGB8(0,200,160),   /* eye dark */
    RGB8(200,200,220), /* detail */
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    
    /* Sub-palette 6: Dr. Vex - orange/dark (96-111) */
    RGB8(0,0,0),
    RGB8(200,180,160), /* skin */
    RGB8(160,140,120), /* skin shadow */
    RGB8(40,30,20),    /* hair dark */
    RGB8(80,60,30),    /* hair */
    RGB8(120,90,40),   /* hair highlight */
    RGB8(60,40,20),    /* coat dark */
    RGB8(100,70,30),   /* coat */
    RGB8(140,100,50),  /* coat light */
    RGB8(200,200,210), /* eye white */
    RGB8(200,150,50),  /* eye amber */
    RGB8(150,100,30),  /* eye dark */
    RGB8(40,40,50),    /* pants */
    RGB8(30,30,40),    /* shoes */
    RGB8(200,100,50),  /* accent */
    RGB8(0,0,0),
    
    /* Sub-palettes 7-15: empty */
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
    RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0), RGB8(0,0,0),
};

void palettes_load(void) {
    dma_to_palette(bg_palette, PALETTE_BG, sizeof(bg_palette));
    dma_to_palette(obj_palette, PALETTE_OBJ, sizeof(obj_palette));
}