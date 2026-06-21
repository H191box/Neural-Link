#include "chapter_title.h"
#include "bg_manager.h"
#include "text_renderer.h"
#include "system.h"
#include "transition.h"

void chapter_title_render(const char *title) {
    if (title == NULL) return;
    
    /* Clear UI and text layers */
    bg_clear_map(BG_UI);
    vn_text_clear_all();
    
    /* Calculate centered position */
    int len = 0;
    while (title[len] != '\0') len++;
    int start_x = (30 - len) / 2;
    if (start_x < 0) start_x = 0;
    
    /* Draw decorative line */
    int i;
    for (i = 4; i < 26; i++) {
        vn_text_draw_char(i, 7, '-', TEXT_COLOR_CYAN);
        vn_text_draw_char(i, 10, '-', TEXT_COLOR_CYAN);
    }
    
    /* Draw title text */
    vn_text_draw_string(start_x, 8, title, TEXT_COLOR_WHITE);
    
    /* "Chapter X" above */
    const ScriptState *ss = script_get_state();
    char ch_num[20];
    int ch = ss ? ss->chapter + 1 : 1;
    ch_num[0] = 'C'; ch_num[1] = 'h'; ch_num[2] = 'a'; ch_num[3] = 'p';
    ch_num[4] = 't'; ch_num[5] = 'e'; ch_num[6] = 'r'; ch_num[7] = ' ';
    if (ch >= 10) {
        ch_num[8] = '0' + ch / 10;
        ch_num[9] = '0' + ch % 10;
        ch_num[10] = '\0';
    } else {
        ch_num[8] = '0' + ch;
        ch_num[9] = '\0';
    }
    int ch_len = 0;
    while (ch_num[ch_len] != '\0') ch_len++;
    int ch_x = (30 - ch_len) / 2;
    vn_text_draw_string(ch_x, 6, ch_num, TEXT_COLOR_CYAN);
}