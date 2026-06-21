#ifndef GIFT_DATA_H
#define GIFT_DATA_H

#include "gba_types.h"

/* Gift item definition for data lookup */
typedef struct {
    u8 type;             /* GIFT_TYPE_* */
    const char *name;    /* Display name (max 15 chars for GBA) */
    const char *desc;    /* Short description */
} GiftItemDef;

/* Number of defined gift items */
#define GIFT_ITEM_COUNT 8

/* Get a gift item definition by type ID */
const GiftItemDef* gift_data_get_by_type(u8 type);

/* Get total number of gift item types */
int gift_data_count(void);

#endif /* GIFT_DATA_H */