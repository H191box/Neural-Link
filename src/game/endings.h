#ifndef ENDINGS_H
#define ENDINGS_H

#include "gba_types.h"

/* Ending IDs */
#define ENDING_REVOLUTION   0  /* A */
#define ENDING_CORPORATE    1  /* B */
#define ENDING_SACRIFICE    2  /* C */
#define ENDING_MERGE        3  /* D */
#define ENDING_HIDDEN_TRUTH 4  /* E */

/* Initialize endings system */
void endings_init(void);

/* Calculate and display the appropriate ending */
void endings_update(void);

/* Render ending screen */
void endings_render(void);

/* Get ending ID based on current game state */
int endings_calculate(void);

#endif /* ENDINGS_H */