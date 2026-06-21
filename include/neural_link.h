#ifndef NEURAL_LINK_H
#define NEURAL_LINK_H

/* =============================================
 * NEURAL LINK - GBA Visual Novel Engine
 * Master Header
 * ============================================= */

/* Version */
#define NL_VERSION_MAJOR 1
#define NL_VERSION_MINOR 0
#define NL_VERSION_STRING "v1.0"

/* Limits */
#define MAX_FLAGS       64
#define MAX_SAVE_SLOTS  3
#define MAX_CHOICES     4
#define MAX_CGS         16

/* Screen */
#define SCREEN_WIDTH    240
#define SCREEN_HEIGHT   160

/* Engine headers */
#include "gba_types.h"
#include "gba_header.h"
#include "system.h"
#include "input.h"
#include "dma.h"
#include "timer.h"
#include "bg_manager.h"
#include "sprite_manager.h"
#include "text_renderer.h"
#include "transition.h"
#include "dialog_box.h"

/* Asset headers */
#include "font.h"
#include "palettes.h"
#include "backgrounds.h"
#include "characters.h"
#include "ui_elements.h"
#include "audio.h"

/* Game headers */
#include "novel_state.h"
#include "faction.h"
#include "flags.h"
#include "characters_game.h"
#include "hacking_minigame.h"
#include "title_screen.h"
#include "chapter_title.h"
#include "gallery.h"
#include "settings.h"
#include "save_load.h"
#include "endings.h"

/* Script headers */
#include "script_engine.h"
#include "script_data.h"

#endif /* NEURAL_LINK_H */