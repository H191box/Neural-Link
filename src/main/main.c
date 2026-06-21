/*
 * Neural Link: Heartsync - GBA Otome Visual Novel
 * Main Entry Point
 */

#include "neural_link.h"

/* Game initialization */
static void game_init(void) {
    /* Hardware init */
    sys_init();
    input_init();
    timer_ms_init();
    audio_init();

    /* Graphics init */
    bg_init();
    spr_init();
    palettes_load();

    /* Load assets */
    backgrounds_load_tiles();
    characters_load_sprites();
    gift_sprites_load();
    ui_elements_load();

    /* Init text renderer (loads font to BG3) */
    vn_text_init();

    /* Init game systems */
    flags_init();
    faction_init();
    game_chars_init();
    gift_init();
    heart_events_init();
    route_init();
    trans_init();
    dialog_init();
    settings_init();
    save_system_init();

    /* Show title screen */
    title_screen_init();
    novel_state_init();
    novel_state_set_state(STATE_TITLE);
}

/* Main game loop */
void main(void) {
    game_init();

    /* Main loop */
    while (1) {
        /* Update input */
        input_update();

        /* Update game state */
        novel_state_update();

        /* Render */
        novel_state_render();

        /* Update sprites */
        spr_update();

        /* Wait for VBlank */
        sys_vblank_wait();
    }
}