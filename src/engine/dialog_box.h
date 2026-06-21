#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H

#include "gba_types.h"
#include "text_renderer.h"

/* Dialog box dimensions (in tiles) */
#define DBOX_X      1
#define DBOX_Y      14
#define DBOX_W      30
#define DBOX_H      6
#define DBOX_TEXT_X 2
#define DBOX_TEXT_Y 15
#define DBOX_TEXT_W 28
#define DBOX_TEXT_H 5

/* Max choices */
#define MAX_CHOICES 4

/* Choice entry */
typedef struct {
    char text[40];
    int label_id;    /* Script label to jump to */
} Choice;

/* Dialog box state */
typedef struct {
    bool visible;
    char speaker[20];
    char text[256];
    Choice choices[MAX_CHOICES];
    int num_choices;
    int selected_choice;
    TypewriterState tw;
    bool waiting_for_input;    /* Text is complete, waiting for A press */
    bool showing_choices;      /* Choices are being shown */
    bool choice_made;          /* Player selected a choice */
    int choice_blink_timer;
} DialogBox;

/* Initialize dialog box system */
void dialog_init(void);

/* Open the dialog box */
void dialog_open(void);

/* Close the dialog box */
void dialog_close(void);

/* Set speaker name */
void dialog_set_speaker(const char *name);

/* Set dialog text (starts typewriter effect) */
void dialog_set_text(const char *text);

/* Add a choice option */
void dialog_add_choice(const char *text, int label_id);

/* Clear all choices */
void dialog_clear_choices(void);

/* Update dialog box (call once per frame) */
void dialog_update(void);

/* Render dialog box (draws box frame and text) */
void dialog_render(void);

/* Check if dialog text is complete */
bool dialog_text_complete(void);

/* Check if a choice has been made */
bool dialog_choice_made(void);

/* Get selected choice index (-1 if none) */
int dialog_get_choice(void);

/* Is dialog box visible? */
bool dialog_visible(void);

/* Clear the text area */
void dialog_clear(void);

#endif /* DIALOG_BOX_H */