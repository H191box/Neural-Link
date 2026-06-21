#ifndef AUDIO_H
#define AUDIO_H

#include "gba_types.h"

/* SFX IDs */
#define SFX_TEXT_ADVANCE  0
#define SFX_CHOICE_SELECT 1
#define SFX_MENU_MOVE     2
#define SFX_HACK_SUCCESS  3
#define SFX_HACK_FAIL     4
#define SFX_TRANSITION    5
#define SFX_CHAPTER       6
#define SFX_COUNT         7

/* Initialize sound system */
void audio_init(void);

/* Play a sound effect */
void sfx_play(int sfx_id);

/* Set sound on/off */
void audio_set_enabled(bool enabled);

/* Check if sound is enabled */
bool audio_get_enabled(void);

#endif /* AUDIO_H */