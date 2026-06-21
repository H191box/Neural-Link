#ifndef NEURAL_LINK_H
#define NEURAL_LINK_H

/* =============================================
 * NEURAL LINK: HEARTSYNC - GBA Otome Visual Novel
 * Master Header
 * =============================================
 * "In a world of neural implants, the deepest
 *  connection is the one between hearts."
 * ============================================= */

/* Version */
#define NL_VERSION_MAJOR 1
#define NL_VERSION_MINOR 1
#define NL_VERSION_STRING "v1.1"
#define NL_SUBTITLE       "HEARTSYNC"

/* Title display */
#define NL_TITLE_LINE1    "NEURAL LINK"
#define NL_TITLE_LINE2    "HEARTSYNC"

/* Limits */
#define MAX_FLAGS       64
#define MAX_SAVE_SLOTS  3
#define MAX_CHOICES     4
#define MAX_CGS         16

/* Screen */
#define SCREEN_WIDTH    240
#define SCREEN_HEIGHT   160

/* =============================================
 * GIFT SYSTEM CONSTANTS
 * ============================================= */

/* Gift type IDs */
#define GIFT_WEAPON  0
#define GIFT_LUXURY  1
#define GIFT_DATA    2
#define GIFT_BOOK    3
#define GIFT_FLOWER  4
#define GIFT_MUSIC   5
#define GIFT_FOOD    6
#define GIFT_TECH    7
#define GIFT_TYPE_COUNT 8

/* Gift inventory size */
#define GIFT_INV_SLOTS  16
#define GIFT_ID_NONE   255

/* Affinity values for gift reactions */
#define GIFT_PREFERRED_AFFINITY  15
#define GIFT_NEUTRAL_AFFINITY     5
#define GIFT_DISLIKED_AFFINITY   -5

/* =============================================
 * AFFINITY / HEART EVENT CONSTANTS
 * ============================================= */

/* Affinity thresholds for heart events */
#define AFFINITY_HEART1    20  /* First heart event */
#define AFFINITY_HEART2    40  /* Second heart event + intimate dialog */
#define AFFINITY_HEART3    60  /* Third heart event */
#define AFFINITY_HEART4    80  /* Fourth heart event */
#define AFFINITY_CONFESSION 95  /* Confession scene */

/* =============================================
 * ROUTE SYSTEM CONSTANTS
 * ============================================= */

/* Route IDs */
#define ROUTE_COMMON    0
#define ROUTE_NOVA      1
#define ROUTE_CHEN      2
#define ROUTE_ARIA      3
#define ROUTE_VEX       4
#define ROUTE_COUNT     5

/* Route lock chapter (end of Chapter 3) */
#define ROUTE_LOCK_CHAPTER  2  /* 0-indexed: Chapter 3 */

/* Auto-lock minimum affinity threshold */
#define ROUTE_AUTO_LOCK_MIN  40

/* =============================================
 * CHARACTER CONSTANTS
 * ============================================= */

/* LI count (excluding protagonist) */
#define LI_COUNT       4
#define LI_START       1  /* First LI index in character array */

/* Personality trait bits (affect dialog options) */
#define TRAIT_PASSIONATE  0x01  /* Nova */
#define TRAIT_CONTROLLED  0x02  /* Chen */
#define TRAIT_CURIOUS     0x04  /* ARIA */
#define TRAIT_GENTLE      0x08  /* Vex */
#define TRAIT_BRAVE       0x10
#define TRAIT_SECRETIVE   0x20

/* =============================================
 * ENDING CONSTANTS
 * ============================================= */

/* Total endings: 4 LIs × 3 endings + True + Secret = 14 */
#define ENDING_PER_LI       3   /* Good, Normal, Bad */
#define ENDING_TRUE         12
#define ENDING_SECRET       13
#define TOTAL_ENDINGS       14

/* Ending sub-types */
#define ENDING_GOOD    0
#define ENDING_NORMAL  1
#define ENDING_BAD     2

/* =============================================
 * HEART EVENT CONSTANTS
 * ============================================= */

#define HEART_EVENTS_PER_LI  5  /* 4 heart events + 1 confession */
#define MAX_HEART_EVENTS     (LI_COUNT * HEART_EVENTS_PER_LI)

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
#include "gift_sprites.h"

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
#include "gift_system.h"
#include "heart_events.h"
#include "route_system.h"

/* Data headers */
#include "gift_data.h"

/* Script headers */
#include "script_engine.h"
#include "script_data.h"

#endif /* NEURAL_LINK_H */