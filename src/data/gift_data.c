#include "gift_data.h"

/*
 * Gift item definitions.
 * These are the 8 gift types with display names and descriptions.
 * One item per type for simplicity (GBA memory constraints).
 */

static const GiftItemDef gift_items[GIFT_ITEM_COUNT] = {
    /* GIFT_WEAPON */
    { GIFT_WEAPON,
      "EMP Grenade",
      "Resistance hardware"
    },
    /* GIFT_LUXURY */
    { GIFT_LUXURY,
      "Silk Scarf",
      "Soft and elegant"
    },
    /* GIFT_DATA */
    { GIFT_DATA,
      "Data Chip",
      "Encrypted info"
    },
    /* GIFT_BOOK */
    { GIFT_BOOK,
      "Old Poetry",
      "Classic verses"
    },
    /* GIFT_FLOWER */
    { GIFT_FLOWER,
      "Synth Rose",
      "Engineered bloom"
    },
    /* GIFT_MUSIC */
    { GIFT_MUSIC,
      "Music Box",
      "Melodic crystal"
    },
    /* GIFT_FOOD */
    { GIFT_FOOD,
      "Tea Set",
      "Calming blend"
    },
    /* GIFT_TECH */
    { GIFT_TECH,
      "Neural Tuner",
      "Precision tool"
    }
};

const GiftItemDef* gift_data_get_by_type(u8 type) {
    if (type >= GIFT_ITEM_COUNT) return NULL;
    return &gift_items[type];
}

int gift_data_count(void) {
    return GIFT_ITEM_COUNT;
}