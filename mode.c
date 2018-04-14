#include <ncurses.h>

// These functions change the powerline style bar at the bottom
//   which shows the mode you using in fancy color

void cmd_mode(WINDOW *modebar) {
  wclear(modebar); wattrset(modebar, COLOR_PAIR(1));
  waddstr(modebar," NORMAL "); wrefresh(modebar);
}

void ins_mode(WINDOW *modebar) {
  wclear(modebar); wattrset(modebar, COLOR_PAIR(2)); 
  waddstr(modebar," INSERT "); wrefresh(modebar);
}
