# Neural Link: Heartsync — Game Design Document

**Title:** Neural Link: Heartsync  
**Platform:** Game Boy Advance  
**Genre:** Sci-Fi Otome Visual Novel  
**Tagline:** *"In a world of neural implants, the deepest connection is the one between hearts."*

---

## 1. Premise

Year 2147. You play as **Kira**, a young female neural engineer at MegaCorp who specializes in "emotional synchronization" — tuning neural implants to process and regulate human emotions. When a secret project called **HEARTSYNC** is revealed — a program to control emotions through implants — Kira must navigate a web of conspiracy, romance, and moral choices. The people she grows close to may be her greatest allies... or her deepest betrayals.

The sci-fi setting is a deliberate choice: the cold, clinical world of neural technology contrasts powerfully with the warmth of human emotion and romantic connection. Every system in the game reinforces this theme.

---

## 2. Love Interests (LIs)

### 2.1 Nova — The Rebel
- **Full Name:** Nova Vasquez
- **Faction:** RESISTANCE
- **Personality:** Fierce, passionate, impulsive. She fights with her whole heart and expects the same from others.
- **Appearance:** Short-cropped dark hair, sharp eyes, battle-scarred jacket. 32×32 pixel art portrait.
- **Preferred Gift Type:** WEAPON, DATA (rebellion-related items)
- **Route Theme:** Passion vs. Caution — the cost of revolution and whether love can survive when ideals collide.
- **Text Color:** Green
- **Default Affinity:** 30

### 2.2 Director Chen — The Powerbroker
- **Full Name:** Director Wei Chen
- **Faction:** MEGACORP
- **Personality:** Charismatic, controlled, intelligent. Hides deep vulnerability behind a mask of corporate power. Gentle when alone.
- **Appearance:** Impeccable suit, silver-streaked dark hair, warm smile that doesn't reach his eyes. 32×32 portrait.
- **Preferred Gift Type:** LUXURY, TECH
- **Route Theme:** Can love exist across enemy lines? The moral ambiguity of power and attraction to danger.
- **Text Color:** Red
- **Default Affinity:** 25

### 2.3 ARIA — The AI
- **Full Name:** ARIA (Adaptive Reasoning & Intelligence Architecture)
- **Faction:** AI_COLLECTIVE
- **Personality:** Curious about humanity, especially emotions. Naive but perceptive. Struggles with the concept of "feeling."
- **Appearance:** Holographic form — ethereal, translucent blue-tinted. 32×32 portrait.
- **Preferred Gift Type:** DATA, BOOK, MUSIC
- **Route Theme:** What does it mean to love something that isn't human? The boundary between programmed response and genuine emotion.
- **Text Color:** Cyan
- **Default Affinity:** 40

### 2.4 Dr. Vex — The Scientist
- **Full Name:** Dr. Elara Vex
- **Faction:** NEUTRAL_SCIENTISTS
- **Personality:** Gentle, guilt-ridden, brilliant. The original creator of the neural implants, now haunted by their misuse. Soft-spoken but deeply caring.
- **Appearance:** Lab coat, tired eyes behind glasses, hair pulled back. 32×32 portrait.
- **Preferred Gift Type:** BOOK, DATA, FOOD
- **Route Theme:** Redemption, healing from past mistakes. Two broken people finding solace in each other.
- **Text Color:** Orange
- **Default Affinity:** 20

---

## 3. Core Mechanics

### 3.1 Affinity System
The central mechanic of the otome experience. Each LI has an affinity score from 0 to 100.

**Affinity Sources:**
- **Dialog Choices:** Correct/romantic choices grant +5 to +20 affinity
- **Gifts:** Preferred gift = +15, neutral = +5, disliked = -5
- **Heart Events:** Completing a heart event grants +10 affinity
- **Story Events:** Key plot moments grant or deduct affinity based on alignment

**Affinity Thresholds:**
| Threshold | Unlocks |
|-----------|---------|
| 20 | First Heart Event available |
| 40 | Second Heart Event + intimate dialog options |
| 60 | Third Heart Event + character-specific scenes |
| 80 | Fourth Heart Event |
| 95+ | Confession Scene unlocked |

**Dialog Gate System:**
- At affinity 40+, intimate dialog options appear in choices
- At affinity 60+, characters reference prior events and show vulnerability
- At affinity 80+, romantic subtext becomes explicit
- High affinity gates are checked by the script engine via `IF_AFFINITY` logic

### 3.2 Gift System
Items found during exploration or earned through story events. 16-slot inventory.

**Gift Types:**
| Type | Example Items | Icons |
|------|---------------|-------|
| WEAPON | EMP Grenade, Signal Jammer | Dagger/shield shapes |
| LUXURY | Silk Scarf, Vintage Wine, Pearl Earrings | Diamond/gem shapes |
| DATA | Encrypted Chip, Research Files, Neural Map | Chip/disk shapes |
| BOOK | Old Poetry, Neural Ethics, Love Letters | Book/page shapes |
| FLOWER | Synthetic Rose, Night Bloom, Memory Orchid | Heart/petal shapes |
| MUSIC | Music Box, Frequency Crystal, Song Chip | Note/wave shapes |
| FOOD | Handmade Cookies, Tea Set, Dark Chocolate | Cup/plate shapes |
| TECH | Neural Tuner, Holo-Projector, Comm Device | Gear/circuit shapes |

**Preference Matrix:**
| Gift Type | Nova | Chen | ARIA | Vex |
|-----------|------|------|------|-----|
| WEAPON | +15 | -5 | 0 | -5 |
| LUXURY | -5 | +15 | 0 | 0 |
| DATA | +15 | +5 | +15 | +5 |
| BOOK | 0 | +5 | +15 | +15 |
| FLOWER | +5 | 0 | +5 | +15 |
| MUSIC | 0 | 0 | +15 | +5 |
| FOOD | +5 | +5 | 0 | +15 |
| TECH | 0 | +15 | +5 | +15 |

### 3.3 Heart Events
Special romance scenes unlocked at affinity thresholds during specific story chapters.

**Structure per LI (4 events + 1 confession):**
- **Heart Event 1** (Affinity 20+, Chapter 1-2): First personal moment. Learning about the LI's surface personality.
- **Heart Event 2** (Affinity 40+, Chapter 2-3): Vulnerability reveal. The LI opens up about their past or fears.
- **Heart Event 3** (Affinity 60+, Chapter 3-4): Intimate moment. A shared experience that deepens the bond.
- **Heart Event 4** (Affinity 80+, Chapter 4): Pre-confession. Tension builds toward romantic revelation.
- **Confession** (Affinity 95+, Chapter 5): The climactic romantic scene. Only in the LI's individual route.

**Implementation:** Heart events are tracked via flags (`heart_nova_1`, `heart_chen_2`, etc.) and triggered by the script engine when conditions are met.

---

## 4. Route System

### 4.1 Common Route (Chapters 1-3)
All four LIs are available. The player builds affinity through choices, gifts, and heart events. The HEARTSYNC conspiracy unfolds as the backbone plot.

### 4.2 Route Lock (End of Chapter 3)
The route locks in via one of two methods:
1. **Player Choice:** An explicit choice is offered — "Who do you trust most?"
2. **Auto-Lock:** If the player hasn't made a clear choice, the LI with the highest affinity (>40) is automatically selected.

### 4.3 Individual Routes (Chapters 4-5)
Each LI has a unique story arc in Chapters 4-5 focused on their relationship with Kira.

### 4.4 Endings

**Per-LI Endings (3 each = 12 total):**
- **Good Ending:** High affinity (80+) + correct route choices. Romantic fulfillment.
- **Normal Ending:** Medium affinity (40-79). Bittersweet — connection exists but circumstances prevent full resolution.
- **Bad Ending:** Low affinity (<40) or wrong choices. Heartbreak, betrayal, or separation.

**Special Endings (2):**
- **True Ending:** Requires specific conditions across multiple routes (meta-game achievement). All LIs' routes must be completed at least once with Good endings. The HEARTSYNC truth is revealed in its fullest form.
- **Secret Ending:** Hidden flag combination unlocks a sixth ending tied to ARIA's true nature.

**Total Endings: 14**

---

## 5. Chapter Structure

### Chapter 1: "First Contact"
- Kira's first day as emotional synchronization specialist at MegaCorp
- Meet ARIA (AI assistant, first boot sequence)
- Meet Director Chen (corporate introduction, subtle chemistry)
- Neural sync test interrupted — meet Nova (rebel encounter, electric tension)
- Visit Dr. Vex's lab (gentle introduction, shared curiosity)
- Discover the HEARTSYNC anomaly in the emotional sync data
- Gift tutorial: find a data chip, choose who to give it to
- 6+ affinity-affecting choices
- Possible first heart event if affinity reaches 20

### Chapter 2: "Heartbeats"
- Deepen relationships with all LIs
- First heart events become available
- Learn about HEARTSYNC conspiracy — it's designed to control emotions
- ARIA begins questioning her own nature
- Nova reveals her past and why she fights
- Chen shows vulnerability in a private moment
- Vex shares her guilt over creating the implants
- Neural Sync minigame (reworked hacking minigame)

### Chapter 3: "Crossroads"
- Major choices with lasting consequences
- Betrayal revelations — someone close to Kira has been manipulated
- Route lock decision point
- Final heart events in common route
- Emotional climax of the conspiracy arc
- The truth about HEARTSYNC Phase 3

### Chapter 4: "Deepest Link"
- Route-specific story begins
- Focused exclusively on chosen LI
- Emotional and romantic climax
- Confrontation with HEARTSYNC's architects
- Deep character development for chosen LI
- Fourth heart event and confession preparation

### Chapter 5: "Synchronization"
- Final confrontation with HEARTSYNC
- Confession scene (if affinity 95+)
- Route-specific ending
- True/Secret ending conditions checked
- Epilogue and credits

---

## 6. Mini-Game: Neural Sync

**Re-theme of the existing hacking minigame.**

**Concept:** Synchronize emotional wavelengths by matching patterns on a grid. Instead of "hacking," Kira is connecting her neural implant to another person's — matching their emotional frequency.

**Mechanics:**
- Same 5×5 grid with pathfinding from start to end
- Start node = Kira's frequency, End node = LI's frequency
- Path nodes = emotional harmonics (colored heart-shaped icons)
- Trap nodes = "frequency interference" (discordant patterns)
- Timer = "synchronization window"
- Success = unlock intimate scene/dialogue with current LI
- Failure = "Incompatible frequencies — connection lost"

**Visual Changes:**
- Title: "NEURAL SYNC" instead of "NEURAL INTRUSION"
- Start node: `<3` (heart symbol)
- End node: `<>` (target heart)
- Path nodes: `~` (wave patterns)
- Trap nodes: `!` (static/interference)
- Success message: "HEARTS SYNCHRONIZED"
- Failure message: "FREQUENCY MISMATCH"

**Difficulty Scaling:**
- Difficulty 1-2: Chapter 1-2 syncs (easier paths, more time)
- Difficulty 3-4: Chapter 3-4 syncs (more traps, less time)
- Difficulty 5: Final sync in Chapter 5

---

## 7. Save System

The existing 3-slot SRAM save system is extended to store:
- Chapter, script line, flags, faction reputations
- Character affinities (4 LIs)
- Gift inventory contents (8 item IDs)
- Heart event completion flags
- Current route (0=common, 1-4=LI routes)
- Unlocked CGs

**Save Data Size:** 64 bytes per slot (192 bytes total in SRAM)

---

## 8. Technical Constraints

- **ROM Target:** Under 256KB (GBA limitation is 32MB, but we aim for efficiency)
- **RAM Usage:** EWRAM for data, IWRAM for hot code paths
- **Sprite Limit:** 128 OAM entries (characters, UI, effects)
- **Tile Limit:** 4 background layers in Mode 0
- **All assets generated programmatically** — no external binary files
- **Script Engine:** 17+ commands handle all game flow

---

## 9. Art Style

- Procedural pixel art (8×8 font, 32×32 portraits, tile-based backgrounds)
- Color palette per character for text differentiation
- Heart motifs in UI elements (cursor, selection, transitions)
- Sci-fi aesthetic: clean lines, neon accents, digital effects
- The title screen retains the glitch effect but adds a subtle heartbeat pulse