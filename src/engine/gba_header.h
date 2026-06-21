#ifndef GBA_HEADER_H
#define GBA_HEADER_H

/* Neural Link - GBA Register Definitions */

/* Memory-mapped I/O base */
#define REG_BASE  0x04000000

/* Display registers */
#define REG_DISPCNT    (*(volatile u16*)(REG_BASE + 0x000))
#define REG_DISPSTAT   (*(volatile u16*)(REG_BASE + 0x004))
#define REG_VCOUNT     (*(volatile u16*)(REG_BASE + 0x006))

/* Display Control bits */
#define MODE_0   0x0000
#define MODE_1   0x0001
#define MODE_2   0x0002
#define MODE_3   0x0003
#define MODE_4   0x0004
#define MODE_5   0x0005

#define BG0_ENABLE  0x0100
#define BG1_ENABLE  0x0200
#define BG2_ENABLE  0x0400
#define BG3_ENABLE  0x0800
#define OBJ_ENABLE  0x1000
#define WIN0_ENABLE 0x2000
#define WIN1_ENABLE 0x4000
#define OBJWIN_ENABLE 0x8000

/* Display Status bits */
#define VBLANK_FLAG    0x0001
#define HBLANK_FLAG    0x0002
#define VCOUNT_FLAG    0x0004
#define VBLANK_IRQ     0x0008
#define HBLANK_IRQ     0x0010
#define VCOUNT_IRQ     0x0020

/* Background registers */
#define REG_BG0CNT (*(volatile u16*)(REG_BASE + 0x008))
#define REG_BG1CNT (*(volatile u16*)(REG_BASE + 0x00A))
#define REG_BG2CNT (*(volatile u16*)(REG_BASE + 0x00C))
#define REG_BG3CNT (*(volatile u16*)(REG_BASE + 0x00E))

#define REG_BG0HOFS  (*(volatile u16*)(REG_BASE + 0x010))
#define REG_BG0VOFS  (*(volatile u16*)(REG_BASE + 0x012))
#define REG_BG1HOFS  (*(volatile u16*)(REG_BASE + 0x014))
#define REG_BG1VOFS  (*(volatile u16*)(REG_BASE + 0x016))
#define REG_BG2HOFS  (*(volatile u16*)(REG_BASE + 0x018))
#define REG_BG2VOFS  (*(volatile u16*)(REG_BASE + 0x01A))
#define REG_BG3HOFS  (*(volatile u16*)(REG_BASE + 0x01C))
#define REG_BG3VOFS  (*(volatile u16*)(REG_BASE + 0x01E))

/* BG Control bits */
#define BG_PRIORITY(n)   ((n) << 0)
#define BG_CHARBASE(n)   ((n) << 2)
#define BG_SCREENBASE(n) ((n) << 8)
#define BG_8BPP          0x0080
#define BG_4BPP          0x0000
#define BG_WRAP          0x2000
#define BG_SIZE0         0x0000  /* 256x256 */
#define BG_SIZE1         0x4000  /* 512x256 */
#define BG_SIZE2         0x8000  /* 256x512 */
#define BG_SIZE3         0xC000  /* 512x512 */

/* Window registers */
#define REG_WIN0H    (*(volatile u16*)(REG_BASE + 0x040))
#define REG_WIN1H    (*(volatile u16*)(REG_BASE + 0x042))
#define REG_WIN0V    (*(volatile u16*)(REG_BASE + 0x044))
#define REG_WIN1V    (*(volatile u16*)(REG_BASE + 0x046))
#define REG_WININ    (*(volatile u16*)(REG_BASE + 0x048))
#define REG_WINOUT   (*(volatile u16*)(REG_BASE + 0x04A))

/* MOSAIC */
#define REG_MOSAIC   (*(volatile u16*)(REG_BASE + 0x04C))

/* Blend control */
#define REG_BLDCNT   (*(volatile u16*)(REG_BASE + 0x050))
#define REG_BLDALPHA (*(volatile u16*)(REG_BASE + 0x052))
#define REG_BLDY     (*(volatile u16*)(REG_BASE + 0x054))

/* Blend modes */
#define BLD_NONE       0
#define BLD_ALPHA      1
#define BLD_WHITE      2
#define BLD_BLACK      3

#define BLD_BG0        0x0001
#define BLD_BG1        0x0002
#define BLD_BG2        0x0004
#define BLD_BG3        0x0008
#define BLD_OBJ        0x0010
#define BLD_BACKDROP   0x0020

/* DMA registers */
#define REG_DMA0SAD (*(volatile u32*)(REG_BASE + 0x0B0))
#define REG_DMA0DAD (*(volatile u32*)(REG_BASE + 0x0B4))
#define REG_DMA0CNT (*(volatile u16*)(REG_BASE + 0x0B8))
#define REG_DMA1SAD (*(volatile u32*)(REG_BASE + 0x0BC))
#define REG_DMA1DAD (*(volatile u32*)(REG_BASE + 0x0C0))
#define REG_DMA1CNT *(volatile u16*)(REG_BASE + 0x0C4)
#define REG_DMA2SAD (*(volatile u32*)(REG_BASE + 0x0C8))
#define REG_DMA2DAD (*(volatile u32*)(REG_BASE + 0x0CC))
#define REG_DMA2CNT *(volatile u16*)(REG_BASE + 0x0D0)
#define REG_DMA3SAD *(volatile u32*)(REG_BASE + 0x0D4)
#define REG_DMA3DAD *(volatile u32*)(REG_BASE + 0x0D8)
#define REG_DMA3CNT *(volatile u16*)(REG_BASE + 0x0DC)

/* DMA control bits */
#define DMA_ENABLE        0x8000
#define DMA_IRQ           0x4000
#define DMA_16            0x0000
#define DMA_32            0x0400
#define DMA_REPEAT        0x0200
#define DMA_NOW           0x0100
#define DMA_DEST_INC      0x0000
#define DMA_DEST_DEC      0x0020
#define DMA_DEST_FIXED    0x0040
#define DMA_DEST_RELOAD   0x0060
#define DMA_SRC_INC       0x0000
#define DMA_SRC_DEC       0x0008
#define DMA_SRC_FIXED     0x0010

/* Timer registers */
#define REG_TM0D   (*(volatile u16*)(REG_BASE + 0x100))
#define REG_TM0CNT *(volatile u16*)(REG_BASE + 0x102)
#define REG_TM1D   (*(volatile u16*)(REG_BASE + 0x104))
#define REG_TM1CNT *(volatile u16*)(REG_BASE + 0x106)
#define REG_TM2D   (*(volatile u16*)(REG_BASE + 0x108))
#define REG_TM2CNT *(volatile u16*)(REG_BASE + 0x10A)
#define REG_TM3D   (*(volatile u16*)(REG_BASE + 0x10C))
#define REG_TM3CNT *(volatile u16*)(REG_BASE + 0x10E)

/* Timer control bits */
#define TIMER_ENABLE       0x0080
#define TIMER_IRQ          0x0040
#define TIMER_CASCADE      0x0040  /* only TM1-3 */
#define TIMER_FREQ_1       0x0000
#define TIMER_FREQ_64      0x0001
#define TIMER_FREQ_256     0x0002
#define TIMER_FREQ_1024    0x0003

/* Keypad registers */
#define REG_KEYINPUT *(volatile u16*)(REG_BASE + 0x130)
#define REG_KEYCNT   *(volatile u16*)(REG_BASE + 0x132)

/* Key bits (active low) */
#define KEY_A       0x0001
#define KEY_B       0x0002
#define KEY_SELECT  0x0004
#define KEY_START   0x0008
#define KEY_RIGHT   0x0010
#define KEY_LEFT    0x0020
#define KEY_UP      0x0040
#define KEY_DOWN    0x0080
#define KEY_R       0x0100
#define KEY_L       0x0200

/* Interrupt registers */
#define REG_IME     *(volatile u16*)(REG_BASE + 0x208)
#define REG_IE      *(volatile u16*)(REG_BASE + 0x200)
#define REG_IF      *(volatile u16*)(REG_BASE + 0x202)

/* Interrupt flags */
#define IRQ_VBLANK  0x0001
#define IRQ_HBLANK  0x0002
#define IRQ_VCOUNT  0x0004
#define IRQ_TIMER0  0x0008
#define IRQ_TIMER1  0x0010
#define IRQ_TIMER2  0x0020
#define IRQ_TIMER3  0x0040
#define IRQ_SERIAL  0x0080
#define IRQ_DMA0    0x0100
#define IRQ_DMA1    0x0200
#define IRQ_DMA2    0x0400
#define IRQ_DMA3    0x0800
#define IRQ_KEYPAD  0x1000
#define IRQ_GAMEPAK 0x2000

/* Sound registers */
#define REG_SND1SWEEP *(volatile u16*)(REG_BASE + 0x060)
#define REG_SND1CNT   *(volatile u16*)(REG_BASE + 0x062)
#define REG_SND1FREQ  *(volatile u16*)(REG_BASE + 0x064)
#define REG_SND2CNT   *(volatile u16*)(REG_BASE + 0x068)
#define REG_SND2FREQ  *(volatile u16*)(REG_BASE + 0x06C)
#define REG_SND3CNT   *(volatile u16*)(REG_BASE + 0x070)
#define REG_SND3FREQ  *(volatile u16*)(REG_BASE + 0x074)
#define REG_SND4CNT   *(volatile u16*)(REG_BASE + 0x078)
#define REG_SND4FREQ  *(volatile u16*)(REG_BASE + 0x07C)
#define REG_SNDCTL    *(volatile u16*)(REG_BASE + 0x080)
#define REG_SNDDMGCNT *(volatile u16*)(REG_BASE + 0x082)
#define REG_SNDDSCNT  *(volatile u16*)(REG_BASE + 0x084)
#define REG_SNDSTAT   *(volatile u16*)(REG_BASE + 0x088)
#define REG_SND1FREQ_L (*(volatile u8*)(REG_BASE + 0x064))
#define REG_SND1FREQ_H (*(volatile u8*)(REG_BASE + 0x065))

/* Sound enable bits */
#define SND_ENABLE    0x0080
#define SND_RIGHT     0x0100
#define SND_LEFT      0x0200
#define SND_TIMER     0x0400
#define SND_RESET_FIFO 0x0800

/* OAM */
#define OAM_BASE      ((u16*)0x07000000)
#define OBJ_BASE      ((u16*)0x06010000)

/* VRAM addresses */
#define VRAM_BASE     0x06000000
#define BG_CHARBLOCK(n)  ((u16*)(VRAM_BASE + (n) * 0x4000))
#define BG_SCREENBLOCK(n) ((u16*)(VRAM_BASE + (n) * 0x2000 + 0x8000))
#define PALETTE_BG    ((u16*)VRAM_BASE)
#define PALETTE_OBJ   ((u16*)(VRAM_BASE + 0x200))

/* EWRAM / IWRAM */
#define EWRAM_BASE    0x02000000
#define IWRAM_BASE    0x03000000

/* SRAM */
#define SRAM_BASE     0x0E000000

/* ROM wait states */
#define REG_WAITCNT *(volatile u16*)(REG_BASE + 0x204)

/* BIOS calls */
#define BIOS_VBlankIntrWait() __asm__ volatile("swi 0x05")

/* VBlank wait macro */
#define VBlankIntrWait() BIOS_VBlankIntrWait()

#endif /* GBA_HEADER_H */