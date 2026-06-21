#ifndef GIFT_SYSTEM_H
#define GIFT_SYSTEM_H

#include "gba_types.h"

/* Gift item definition (compact for SRAM) */
typedef struct {
    u8 id;            /* Gift instance ID */
    u8 type;          /* GIFT_TYPE_* */
    const char *name; /* Display name */
    const char *desc; /* Short description */
} GiftItem;

/* Gift inventory state */
#define GIFT_INV_SLOTS 16
#define GIFT_ID_NONE   255

/* Initialize gift system */
void gift_init(void);

/* Add a gift to inventory by type. Returns slot index or -1 if full */
int gift_add(u8 gift_type);

/* Remove a gift from inventory slot. Returns the gift type removed, or GIFT_ID_NONE */
u8 gift_remove(int slot);

/* Get gift item data from inventory slot */
const GiftItem* gift_get(int slot);

/* Check if inventory has a specific gift type. Returns slot or -1 */
int gift_find_type(u8 gift_type);

/* Give a gift from inventory slot to a character (LI). Returns affinity delta */
s8 gift_give(int slot, int li_id);

/* Check gift preference for a character. Returns affinity delta. */
s8 gift_check_preference(int li_id, u8 gift_type);

/* Get count of items in inventory */
int gift_count(void);

/* Get all inventory item IDs for save/load */
void gift_get_inventory(u8 ids[GIFT_INV_SLOTS]);

/* Restore inventory from save data */
void gift_set_inventory(const u8 ids[GIFT_INV_SLOTS]);

/* ---- Gift Selection UI ---- */

/* Initialize gift selection UI (call when entering STATE_GIFT_SELECT) */
void gift_ui_init(void);

/* Update gift selection UI */
void gift_ui_update(void);

/* Render gift selection UI */
void gift_ui_render(void);

/* Check if gift selection is done (item chosen or cancelled) */
bool gift_ui_is_done(void);

/* Get the result: slot index of chosen gift, or -1 if cancelled */
int gift_ui_get_result(void);

/* Get the reaction message from the last gift given */
const char* gift_ui_get_reaction_msg(void);

#endif /* GIFT_SYSTEM_H */