#include "audio.h"
#include "gba_header.h"

static bool sound_enabled = TRUE;

void audio_init(void) {
    /* Enable sound hardware */
    REG_SNDSTAT = SND_ENABLE;
    REG_SNDDMGCNT = 0x770F; /* Full volume */
    sound_enabled = TRUE;
}

void sfx_play(int sfx_id) {
    if (!sound_enabled) return;
    (void)sfx_id;
    /* 
     * Sound effects are stubbed for now.
     * To implement: use GBA's 4 PSG channels or DMA channels.
     * Example for a simple beep on channel 1:
     * 
     * REG_SND1SWEEP = 0;
     * REG_SND1CNT = 0x80; // Square wave, 50% duty
     * REG_SND1FREQ = (2048 - freq) | 0x8000; // frequency + enable
     * 
     * Different sfx_id values would play different tones/effects.
     */
}

void audio_set_enabled(bool enabled) {
    sound_enabled = enabled;
    if (!enabled) {
        REG_SNDSTAT = 0;
    } else {
        REG_SNDSTAT = SND_ENABLE;
    }
}

bool audio_get_enabled(void) {
    return sound_enabled;
}