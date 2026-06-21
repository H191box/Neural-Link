#include "gift_system.h"
#include "gift_data.h"
#include "characters_game.h"
#include "flags.h"
#include "input.h"
#include "bg_manager.h"
#include "text_renderer.h"
#include "system.h"

/* ---- Inventory ---- */

static u8 inventory[GIFT_INV_SLOTS];  /* Gift type per slot, GIFT_ID_NONE = empty */
static int gift_unique_id_counter = 0;

/* UI state */
static int gift_cursor = 0;
static int gift_scroll = 0;
static bool gift_ui_done = FALSE;
static int gift_ui_result = -1;
static char gift_reaction_buf[42];
static int gift_target_li = 0;

void gift_init(void) {
    int i;
    for (i = 0; i < GIFT_INV_SLOTS; i++) {
        inventory[i] = GIFT_ID_NONE;
    }
    gift_unique_id_counter = 0;
    gift_cursor = 0;
    gift_scroll = 0;
    gift_ui_done = FALSE;
    gift_ui_result = -1;
    gift_reaction_buf[0] = '\0';
    gift_target_li = 0;
}

int gift_add(u8 gift_type) {
    int i;
    for (i = 0; i < GIFT_INV_SLOTS; i++) {
        if (inventory[i] == GIFT_ID_NONE) {
            inventory[i] = gift_type;
            return i;
        }
    }
    return -1; /* Inventory full */
}

u8 gift_remove(int slot) {
    if (slot < 0 || slot >= GIFT_INV_SLOTS) return GIFT_ID_NONE;
    u8 type = inventory[slot];
    inventory[slot] = GIFT_ID_NONE;
    return type;
}

const GiftItem* gift_get(int slot) {
    if (slot < 0 || slot >= GIFT_INV_SLOTS) return NULL;
    if (inventory[slot] == GIFT_ID_NONE) return NULL;
    return gift_data_get_by_type(inventory[slot]);
}

int gift_find_type(u8 gift_type) {
    int i;
    for (i = 0; i < GIFT_INV_SLOTS; i++) {
        if (inventory[i] == gift_type) return i;
    }
    return -1;
}

s8 gift_give(int slot, int li_id) {
    if (slot < 0 || slot >= GIFT_INV_SLOTS) return 0;
    if (inventory[slot] == GIFT_ID_NONE) return 0;

    u8 type = inventory[slot];

    /* Get affinity delta from character preference system */
    s8 delta = game_chars_gift_preference(li_id, type);

    /* Get reaction message */
    const char *msg = game_chars_gift_reaction_msg(li_id, type);
    {
        int i = 0;
        while (i < 41 && msg[i] != '\0') {
            gift_reaction_buf[i] = msg[i];
            i++;
        }
        gift_reaction_buf[i] = '\0';
    }

    /* Apply affinity */
    game_chars_modify_affinity(li_id, delta);

    /* Set flag for gift given */
    {
        char flag_name[24];
        const char *li_names[] = {"", "nova", "chen", "aria", "vex"};
        flag_name[0] = '\0';
        /* Simple flag: "gift_nova_1" etc - using a counter */
        flag_set("gift_given", TRUE);
    }

    /* Remove from inventory */
    inventory[slot] = GIFT_ID_NONE;

    return delta;
}

s8 gift_check_preference(int li_id, u8 gift_type) {
    return game_chars_gift_preference(li_id, gift_type);
}

int gift_count(void) {
    int count = 0;
    int i;
    for (i = 0; i < GIFT_INV_SLOTS; i++) {
        if (inventory[i] != GIFT_ID_NONE) count++;
    }
    return count;
}

void gift_get_inventory(u8 ids[GIFT_INV_SLOTS]) {
    int i;
    for (i = 0; i < GIFT_INV_SLOTS; i++) {
        ids[i] = inventory[i];
    }
}

void gift_set_inventory(const u8 ids[GIFT_INV_SLOTS]) {
    int i;
    for (i = 0; i < GIFT_INV_SLOTS; i++) {
        inventory[i] = ids[i];
    }
}

/* ---- Gift Selection UI ---- */

void gift_ui_init(void) {
    gift_cursor = 0;
    gift_scroll = 0;
    gift_ui_done = FALSE;
    gift_ui_result = -1;
    gift_reaction_buf[0] = '\0';

    /* Find first non-empty slot */
    {
        int i;
        for (i = 0; i < GIFT_INV_SLOTS; i++) {
            if (inventory[i] != GIFT_ID_NONE) {
                gift_cursor = i;
                break;
            }
        }
    }
}

void gift_ui_update(void) {
    if (gift_ui_done) return;

    if (input_pressed(KEY_UP)) {
        /* Find previous non-empty slot */
        int prev = gift_cursor - 1;
        while (prev >= 0 && inventory[prev] == GIFT_ID_NONE) prev--;
        if (prev >= 0) {
            gift_cursor = prev;
            sfx_play(SFX_MENU_MOVE);
        }
    }
    if (input_pressed(KEY_DOWN)) {
        /* Find next non-empty slot */
        int next = gift_cursor + 1;
        while (next < GIFT_INV_SLOTS && inventory[next] == GIFT_ID_NONE) next++;
        if (next < GIFT_INV_SLOTS) {
            gift_cursor = next;
            sfx_play(SFX_MENU_MOVE);
        }
    }

    if (input_pressed(KEY_A)) {
        if (inventory[gift_cursor] != GIFT_ID_NONE) {
            gift_ui_result = gift_cursor;
            gift_ui_done = TRUE;
            sfx_play(SFX_CHOICE_SELECT);
        }
    }
}

void gift_ui_render(void) {
    int i;
    int y = 2;

    /* Title */
    vn_text_draw_string(7, 0, "SELECT A GIFT", TEXT_COLOR_YELLOW);
    vn_text_draw_string(1, 1, "<3", TEXT_COLOR_RED); /* Heart icon */

    /* List items (up to 10 visible) */
    int visible_start = 0;
    int visible_end = GIFT_INV_SLOTS;
    if (visible_end > 10) visible_end = 10;

    for (i = visible_start; i < visible_end; i++) {
        int screen_y = y + (i - visible_start);
        if (inventory[i] != GIFT_ID_NONE) {
            const GiftItem *item = gift_data_get_by_type(inventory[i]);
            if (item) {
                u16 color = TEXT_COLOR_WHITE;
                if (i == gift_cursor) {
                    color = TEXT_COLOR_YELLOW;
                    /* Cursor */
                    if (system_get_frame() & 16) {
                        vn_text_draw_char(1, screen_y, '>', TEXT_COLOR_YELLOW);
                    }
                }
                vn_text_draw_string(3, screen_y, item->name, color);
            }
        } else {
            vn_text_draw_string(3, screen_y, "--- empty ---", TEXT_COLOR_GRAY);
        }
    }

    /* Instructions */
    vn_text_draw_string(1, 14, "A:Give  B:Back", TEXT_COLOR_GRAY);
}

bool gift_ui_is_done(void) {
    return gift_ui_done;
}

int gift_ui_get_result(void) {
    return gift_ui_result;
}

const char* gift_ui_get_reaction_msg(void) {
    return gift_reaction_buf;
}