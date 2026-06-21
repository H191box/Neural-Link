#include "system.h"

static u32 frame_counter = 0;
static volatile bool vblank_flag = FALSE;

/* VBlank interrupt handler */
void VBlankHandler(void) __attribute__((interrupt("IRQ")));
void VBlankHandler(void) {
    vblank_flag = TRUE;
    frame_counter++;
    REG_IF = IRQ_VBLANK;
}

/* Initialize GBA for Mode 0 text mode */
void sys_init(void) {
    /* Set up interrupt handler */
    REG_IME = 0;
    REG_IE = IRQ_VBLANK;
    REG_DISPSTAT = VBLANK_IRQ;
    REG_IME = 1;

    /* Mode 0: 4 text BG layers, enable all 4 BGs */
    REG_DISPCNT = MODE_0 | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | BG3_ENABLE;

    /* Default BG settings - all disabled initially, set up in bg_manager */
    REG_BG0CNT = 0;
    REG_BG1CNT = 0;
    REG_BG2CNT = 0;
    REG_BG3CNT = 0;

    /* Clear BG offsets */
    REG_BG0HOFS = 0; REG_BG0VOFS = 0;
    REG_BG1HOFS = 0; REG_BG1VOFS = 0;
    REG_BG2HOFS = 0; REG_BG2VOFS = 0;
    REG_BG3HOFS = 0; REG_BG3VOFS = 0;

    /* Clear OAM */
    my_memset((void*)OAM_BASE, 0, 1024);

    /* Disable windows initially */
    REG_WIN0H = 0;
    REG_WIN0V = 0;
    REG_WININ = 0;
    REG_WINOUT = 0x1F; /* All layers in outside window */

    /* No blending initially */
    REG_BLDCNT = 0;
    REG_BLDALPHA = 0;
    REG_BLDY = 0;

    /* Enable sound (for SFX stubs) */
    REG_SNDSTAT = SND_ENABLE;
    REG_SNDDMGCNT = 0x770F; /* Full volume both sides */
    REG_SNDDSCNT = 0;

    frame_counter = 0;
    vblank_flag = FALSE;
}

/* Busy-wait for VBlank using BIOS SWI */
void sys_vblank_wait(void) {
    VBlankIntrWait();
}

u32 sys_get_frame(void) {
    return frame_counter;
}

void sys_set_frame(u32 f) {
    frame_counter = f;
}

bool sys_in_vblank(void) {
    return vblank_flag;
}

void sys_reset_frame(void) {
    frame_counter = 0;
}