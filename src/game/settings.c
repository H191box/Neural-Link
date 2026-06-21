#include "settings.h"
#include "input.h"
#include "text_renderer.h"
#include "audio.h"
#include "system.h"
#include "bg_manager.h"
#include "dialog_box.h"

typedef enum {
    SET_TEXT_SPEED = 0,
    SET_SOUND,
    SET_COUNT
} SettingItem;

static int setting_cursor = 0;
static int text_speed_option = 1; /* 0=slow, 1=normal, 2=fast */
static bool sound_on = TRUE;
static int settings_anim = 0;

static const char *speed_names[] = {"SLOW", "NORMAL", "FAST"};

void settings_init(void) {
    setting_cursor = 0;
    text_speed_option = 1;
    sound_on = audio_get_enabled();
    settings_anim = 0;
    
    bg_clear_map(BG_UI);
    vn_text_clear_all();
    bg_enable(BG_DIALOG, FALSE);
}

void settings_update(void) {
    settings_anim++;
    
    if (input_pressed(KEY_UP)) {
        setting_cursor--;
        if (setting_cursor < 0) setting_cursor = SET_COUNT - 1;
        sfx_play(SFX_MENU_MOVE);
    }
    if (input_pressed(KEY_DOWN)) {
        setting_cursor++;
        if (setting_cursor >= SET_COUNT) setting_cursor = 0;
        sfx_play(SFX_MENU_MOVE);
    }
    
    if (input_pressed(KEY_LEFT) || input_pressed(KEY_RIGHT)) {
        sfx_play(SFX_MENU_MOVE);
        switch (setting_cursor) {
            case SET_TEXT_SPEED:
                if (input_pressed(KEY_RIGHT)) text_speed_option = (text_speed_option + 1) % 3;
                else text_speed_option = (text_speed_option + 2) % 3;
                /* Apply speed */
                switch (text_speed_option) {
                    case 0: vn_text_set_speed(TEXT_SPEED_SLOW); break;
                    case 1: vn_text_set_speed(TEXT_SPEED_NORMAL); break;
                    case 2: vn_text_set_speed(TEXT_SPEED_FAST); break;
                }
                break;
            case SET_SOUND:
                sound_on = !sound_on;
                audio_set_enabled(sound_on);
                break;
        }
    }
}

void settings_render(void) {
    /* Title */
    vn_text_draw_string(10, 2, "SETTINGS", TEXT_COLOR_CYAN);
    
    /* Text Speed */
    u16 speed_color = (setting_cursor == SET_TEXT_SPEED) ? TEXT_COLOR_YELLOW : TEXT_COLOR_WHITE;
    vn_text_draw_string(3, 6, "Text Speed:", speed_color);
    u16 val_color = (setting_cursor == SET_TEXT_SPEED) ? TEXT_COLOR_GREEN : TEXT_COLOR_GRAY;
    vn_text_draw_string(18, 6, speed_names[text_speed_option], val_color);
    
    /* Left/right arrows for text speed */
    if (setting_cursor == SET_TEXT_SPEED && (settings_anim & 16)) {
        vn_text_draw_char(16, 6, '<', TEXT_COLOR_YELLOW);
        vn_text_draw_char(25, 6, '>', TEXT_COLOR_YELLOW);
    }
    
    /* Sound */
    u16 snd_color = (setting_cursor == SET_SOUND) ? TEXT_COLOR_YELLOW : TEXT_COLOR_WHITE;
    vn_text_draw_string(3, 9, "Sound:", snd_color);
    u16 snd_val = (setting_cursor == SET_SOUND) ? TEXT_COLOR_GREEN : TEXT_COLOR_GRAY;
    vn_text_draw_string(18, 9, sound_on ? "ON" : "OFF", snd_val);
    
    /* Cursor */
    if (settings_anim & 16) {
        int cy = 6 + setting_cursor * 3;
        vn_text_draw_char(1, cy, '>', TEXT_COLOR_YELLOW);
    }
    
    /* Instructions */
    vn_text_draw_string(3, 15, "L/R:Change  B:Back", TEXT_COLOR_GRAY);
}