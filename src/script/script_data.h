#ifndef SCRIPT_DATA_H
#define SCRIPT_DATA_H

#include "gba_types.h"

/* Script IDs */
#define SCRIPT_CH1 0
#define SCRIPT_CH2 1
#define SCRIPT_CH3 2
#define SCRIPT_CH4_NOVA 3
#define SCRIPT_CH4_CHEN 4
#define SCRIPT_CH4_ARIA 5
#define SCRIPT_CH4_VEX  6
#define SCRIPT_CH5_NOVA 7
#define SCRIPT_CH5_CHEN 8
#define SCRIPT_CH5_ARIA 9
#define SCRIPT_CH5_VEX  10
#define SCRIPT_COUNT    11

/* Max lines per script (increased for full otome chapters) */
#define SCRIPT_MAX_LINES 200

/* Get script lines and count */
const char** script_get(int script_id, int *out_count);

#endif /* SCRIPT_DATA_H */