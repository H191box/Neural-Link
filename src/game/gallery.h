#ifndef GALLERY_H
#define GALLERY_H

#include "gba_types.h"

#define MAX_CGS 16

/* Initialize gallery */
void gallery_init(void);

/* Unlock a CG */
void gallery_unlock(int cg_id);

/* Check if CG is unlocked */
bool gallery_is_unlocked(int cg_id);

/* Update gallery (handle input) */
void gallery_update(void);

/* Render gallery screen */
void gallery_render(void);

#endif /* GALLERY_H */