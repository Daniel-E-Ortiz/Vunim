/*
  Authors: Carlos Ventura, Michael McBride, Laura Godinez, Daniel Ortiz and Michael Salinas
*/
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "readfile.h"
#include "mode.h"
#include "save.h"
#include "edit.h"
#include "insert.h"
#include "delete.h"
#include "copypaste.h"
#include "searchreplace.h"
#include "buffer.h"
#include "nav.h"
#include "debug.h"

int main(int argc, char **argv) {
  // check for correct command syntax
  if(argc != 2) {
    fprintf(stderr, "Usage: %s <a txt file>\n", argv[0]);
  }
  // try to open input file
  FILE *fp;
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    perror("Cannot open input file");
    exit(1);
  }

  // start curses mode
  initscr();
  cbreak();
  noecho();
  int ROWS, COLS;
  getmaxyx(stdscr, ROWS, COLS);

  // print title line in editor
  bkgd('_');  // makes title separator + easy to tell if used stdscr func err
  mvprintw(0, (COLS-28-(int)strlen(argv[1]))/2,
      "Group number: 8  File name: %s\n", argv[1]);
  refresh();

  // create windows and set color pairs
  start_color(); use_default_colors();
  init_pair(1, COLOR_RED, COLOR_GREEN);
  init_pair(2, COLOR_WHITE, COLOR_BLUE);
  init_pair(3, COLOR_BLACK, COLOR_BLACK);
  WINDOW *editor = newwin(ROWS-3, COLS, 1, 0); keypad(editor, TRUE);
  WINDOW *command = newwin(1, COLS, ROWS-1, 0);
  WINDOW *modebar = newwin(1, COLS, ROWS-2, 0);
  wbkgd(modebar, COLOR_PAIR(3)); cmd_mode(modebar);

  // create buffer from text file then refresh to screen
  BUFFER *buff = readfile(fp);
  buff->y=0; buff->x=0; buff->offset=0;  // initialize position to zero
  buff->copybuff=NULL; // copybuffer for copy and paste functions set null
  getmaxyx(editor, buff->ROWS, buff->COLS);
  refreshBuff(editor,buff); wmove(editor,0,0);

  int ch, last = 'p';
  bool flag=true;      // flag for while loop, set to false upon :q
  char *str = calloc(4, sizeof(char));   // for number of chars to copy
  while(flag) {
    //debug line
    D(mvprintw(0,0," off%d R%d C%d L%d y%d x%d ",
        buff->offset, buff->ROWS, buff->COLS,buff->num_lines,buff->y,buff->x);
        refresh(););
    // maps to movement, i for edit, or : for cmd
    ch = wgetch(editor);
    wclear(command); wrefresh(command); // clr cmd line after next action
    // redo command available for p, o, and d
    if(ch == '.') { ch = ':'; ungetch(last); }
    // vim like movement through the file contents only
    if(ch == KEY_LEFT || ch == 'h' || ch == KEY_RIGHT || ch == 'l' ||
       ch == KEY_UP   || ch == 'k' || ch == KEY_DOWN  || ch == 'j') {
      buff = navigate(buff, editor, ch, 2);
    }
    // editing key
    else if(ch == 'i') {
      ins_mode(modebar);    // set modebar to insert
      buff = edit_text(buff, editor);
      cmd_mode(modebar);    // set modebar to cmd mode
    }
    // command key
    else if(ch == ':') {
      // first few lines show command at bottom as you type
      waddch(command, ch);
      echo();
      ch=wgetch(command);
      switch(ch) {
        case 'c' :  // copy command; syntax  :c<number>
          wgetnstr(command,str, 3);  // n version prevents more than 3 chars
          if(atoi(str)) //&& !isdigit(str))
            buff = copy(editor, atoi(str), buff);
          break;
        case 'p' :  // paste command
          last = 'p';
          paste(editor, buff); wmove(editor,buff->y,buff->x);
          break;
        case 'o' :  // insert line function
          last = 'o';
          buff = insert(editor, buff); buff->x=0; wmove(editor, buff->y++,0);
          break;
        case 'd' :  // placeholder for delete line function
          last = 'd';
          if(buff->num_lines == 1) { // case of one non-empty line
            buff = insert(editor,buff); buff->y++;
            buff = delete(editor,buff); buff->y--;
            break;
          }
          buff = delete(editor, buff); buff->x=0;
          if(buff->y!=0)
            wmove(editor, --buff->y, 0);
          break;
        case 'r' :  // placeholder for  search and search_replace
          buff = search_replace(buff, command, editor);
          break;
        case 'w' :  // for save function
          save_to_file(buff, argv[1]);
          break;
        case 'q' :  // quit vunim
          flag=false; break;
        default:
          wprintw(command, "   Error: invalid command"); wrefresh(command);
          break;
      }
    }
    noecho(); // turn echoing back off
    refreshBuff(editor,buff); wmove(editor,buff->y,buff->x);
  }
  // let all of the pointers freee
  if(str) free(str);
  if(buff->copybuff) free(buff->copybuff);
  for(int i = 0; i < buff->num_lines; i++)
    free(buff->lines[i]);
  free(buff->lines);
  free(buff);
  endwin();
  return 0;
}
