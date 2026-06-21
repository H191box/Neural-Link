#ifndef SYSTEM_H
#define SYSTEM_H

#include "gba_types.h"
#include "gba_header.h"

/* Initialize GBA hardware for Mode 0 */
void sys_init(void);

/* Wait for next VBlank */
void sys_vblank_wait(void);

/* Get current frame counter */
u32 sys_get_frame(void);

/* Set frame counter */
void sys_set_frame(u32 f);

/* Check if in VBlank */
bool sys_in_vblank(void);

/* Reset frame counter */
void sys_reset_frame(void);

#endif /* SYSTEM_H */