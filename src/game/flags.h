#ifndef FLAGS_H
#define FLAGS_H

#include "gba_types.h"

#define MAX_FLAGS 64

/* Flag name max length */
#define FLAG_NAME_LEN 16

/* Initialize flag system */
void flags_init(void);

/* Set a named flag */
void flag_set(const char *name, bool value);

/* Get a named flag (returns FALSE if not found) */
bool flag_get(const char *name);

/* Check if a flag exists */
bool flag_exists(const char *name);

/* Clear all flags */
void flags_clear(void);

/* Get flag index by name (-1 if not found) */
int flag_find(const char *name);

/* Set flag by index */
void flag_set_index(int index, bool value);

/* Get flag by index */
bool flag_get_index(int index);

#endif /* FLAGS_H */