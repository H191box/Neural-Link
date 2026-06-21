#ifndef SCRIPT_DATA_H
#define SCRIPT_DATA_H

#include "gba_types.h"

/* Script IDs */
#define SCRIPT_CH1 0
#define SCRIPT_CH2 1
#define SCRIPT_CH3 2
#define SCRIPT_CH4 3
#define SCRIPT_CH5 4
#define SCRIPT_COUNT 5

/* Get script lines and count */
const char** script_get(int script_id, int *out_count);

#endif /* SCRIPT_DATA_H */