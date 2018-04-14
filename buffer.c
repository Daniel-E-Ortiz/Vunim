#include <curses.h>
#include "buffer.h"

// This function refreshes the buffer to the editor window
void refreshBuff(WINDOW *editor, BUFFER *buff) {
  wclear(editor); wrefresh(editor);
  int i = 0+buff->offset; // how many lines have been pushed up past window
  // print from offset to the end of the window size to window
  for(; i<buff->num_lines && i-buff->offset<buff->ROWS ; i++) {
    waddnstr(editor, buff->lines[i], COLS);
  }
  // I like the little tildes that vim puts in on lines past your file (=
  for(i++; i<=buff->ROWS; i++) { wmove(editor, i-1,0); waddch(editor,'~'); }
  wrefresh(editor);
}
