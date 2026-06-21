#ifndef ENDINGS_H
#define ENDINGS_H

#include "gba_types.h"

/* Ending sub-types */
#define ENDING_GOOD    0
#define ENDING_NORMAL  1
#define ENDING_BAD     2

/* Special ending IDs */
#define ENDING_TRUE    12  /* Requires all 4 routes completed */
#define ENDING_SECRET  13  /* Hidden flag combination */

/* Total endings: 4 LIs × 3 + True + Secret = 14 */
#define TOTAL_ENDINGS  14

/* Initialize endings system */
void endings_init(void);

/* Calculate and display the appropriate ending */
void endings_update(void);

/* Render ending screen */
void endings_render(void);

/* Get ending ID based on current game state */
int endings_calculate(void);

#endif /* ENDINGS_H */