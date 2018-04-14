#include <ncurses.h>
#include <string.h>
#include "buffer.h"

// This function controls movement in both normal and edit mode
//
//    eol argument controls end of line movement
//      normal mode   eol = 2
//      edit mode     eol = 1
//
BUFFER* navigate(BUFFER *buff, WINDOW *editor, int ch, int eol) {
    if(ch == KEY_LEFT || ch == 'h') { // always move left if >0
      if(buff->x>0) wmove(editor,buff->y,--buff->x);
      else beep();
    }
    else if(ch == KEY_RIGHT || ch == 'l') { // move to the right
      if(buff->x<COLS &&
          buff->x<(int)strlen(buff->lines[buff->y+buff->offset])-eol) {
        if(eol != 1 || buff->x<buff->COLS-1) {
          wmove(editor,buff->y,++buff->x);
        }
      }
      else beep();
    }
    else if(ch == KEY_UP || ch == 'k') {
      if(buff->y==0 && buff->offset>0) { // need to scroll up
        if(buff->lines[buff->y-1+buff->offset][0] == '\n')
          buff->x=0;
        else if(buff->x>((int)strlen(buff->lines[buff->y-1+buff->offset]))-eol)
          buff->x=((int)strlen(buff->lines[buff->y-1+buff->offset]))-eol;
        buff->offset--;
      }
      else if(buff->y>0 &&
          buff->lines[buff->y-1+buff->offset][0] == '\n') { // x=0 if empty
        buff->x=0; wmove(editor, --buff->y, buff->x);
      }
      else if(buff->y>0 &&
          buff->x<((int)strlen(buff->lines[buff->y-1+buff->offset]))-eol)
        // move straight up
        wmove(editor,--buff->y,buff->x);
      else if(buff->y>0) {
        buff->y--; buff->x=((int)strlen(buff->lines[buff->y+buff->offset])-eol);
        wmove(editor, buff->y, buff->x);
      }
      else beep();
    }
    else if(ch == KEY_DOWN || ch == 'j') {
      if(buff->y+1==buff->ROWS &&
          buff->offset+buff->y<buff->num_lines-1) { // need to scroll down
        if(buff->lines[buff->y+1+buff->offset][0] == '\n')
          buff->x=0;
        else if(buff->x>((int)strlen(buff->lines[buff->y+1+buff->offset]))-eol)
          buff->x=((int)strlen(buff->lines[buff->y+1+buff->offset]))-eol;
        buff->offset++;
      }
      else if(buff->offset+buff->y<buff->num_lines-1 &&
          buff->lines[buff->y+1+buff->offset][0] == '\n') { // x=0 if empty
        buff->x=0; wmove(editor, ++buff->y, buff->x);
      }
      else if(buff->offset+buff->y<buff->num_lines-1 &&
          buff->x<((int)strlen(buff->lines[buff->y+1+buff->offset]))-eol)
        // move straight down
        wmove(editor,++buff->y,buff->x);
      else if (buff->offset+buff->y<(buff->num_lines-1)) { // shorter line
        buff->y++;
        buff->x=((int)strlen(buff->lines[buff->y+buff->offset])-eol);
        wmove(editor, buff->y, buff->x);
      }
      else beep();
    }
    return buff;
}
