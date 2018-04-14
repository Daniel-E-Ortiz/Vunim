#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "edit.h"
#include "insert.h"
#include "copypaste.h"
#include "delete.h"
#include "nav.h"
#include "debug.h"

BUFFER* edit_text(BUFFER *buff, WINDOW *editor){ //bring in current cursor position
    int ch;
    wmove(editor,buff->y,buff->x); //wrefresh(editor);

    bool flag=true;
    while(flag){
        ch = wgetch(editor);
        int strLength = (int)strlen(buff->lines[buff->y+buff->offset]);
        //curr str length in curr row
        switch(ch){
          case 27: //if ESC entered, exit loop
            flag = false;
            //since this ends insert mode, freeing copy buff here
            free(buff->copybuff); buff->copybuff = NULL;
            break;
          case 127:
          case KEY_BACKSPACE:{
            if(buff->x==0 && buff->y+buff->offset==0) {
              continue;
            }
            else if(buff->x==0 && buff->y > 0 && buff->lines[buff->y+buff->offset][0] == '\n'){
              //combining two lines together
              buff = delete(editor, buff);
            }
            else if(buff->x==0) {
              //merge lines together if fit in width of buffer
              int prevLength = ((int)strlen(buff->lines[buff->y-1]))-1;
              if(prevLength+strLength-1 <= COLS){ //-1 only accounts for ONE '\n'
                buff = copy(editor, strLength, buff);
                buff = delete(editor, buff);
                --buff->y;
                buff->x = prevLength;
                wmove(editor, buff->y, buff->x);
                paste(editor, buff);

                //delete y+1 line
                //refreshBuff(editor, buff);
                //wmove(editor, y, x);
             }
              break;
            } // case where line is not empyty but cursor at beginning
            else{ //removing one character
              //shift characters over to left ONCE
              //overwrite old '\n' with NULL (call strlen)
              for(int i = buff->x; i < strLength; i++){
                buff->lines[buff->y+buff->offset][i-1] = buff->lines[buff->y+buff->offset][i];
              }
              buff->lines[buff->y+buff->offset][strLength-1] = '\0';
              buff->x--;
            }
            break;
          }
          case KEY_ENTER:
          case 10:
            if(buff->x == ((int)strlen(buff->lines[buff->y+buff->offset]))-1){
              //call Mike's new line function just to
              buff->y++; buff->x = 0;
              wmove(editor, buff->y, buff->x);
              buff = insert(editor, buff);
              wmove(editor, buff->y, buff->x);
            }
            else if(buff->x == 0) {
              buff = insert(editor,buff);
              wmove(editor, buff->y++, 0);
            }
            else{
              //copy what's under cursor + rest of line (w/ copy func)
              buff = copy(editor, strLength-buff->x-1, buff);
              //where cursor is BEFORE enter, put '\n'
              //remove copied text from previous line
              buff->lines[buff->y+buff->offset][buff->x] = '\n';
              for(int i = buff->x+1; i <= strLength; i++)
                buff->lines[buff->y+buff->offset][i] = '\0';

              buff->y++; buff->x = 0;
              buff = insert(editor, buff);
              //paste into new line and add '\n'+'\0'
              paste(editor, buff);
              buff->lines[buff->y+buff->offset][strLength] = '\n';
              buff->lines[buff->y+buff->offset][strLength+1] = '\0';
            }
            break;
            case KEY_BTAB:
            case KEY_CTAB:
            case KEY_STAB:
            case KEY_CATAB:
            case 9:
              //insert tab aka four spaces
              if(strLength+3 <= buff->COLS){
                for(int i = strLength+3; i > buff->x; --i){
                  buff->lines[buff->y+buff->offset][i] = buff->lines[buff->y+buff->offset][i-4];
                }
                for(int i = buff->x; i < buff->x+4; i++)
                  buff->lines[buff->y+buff->offset][i] = ' ';
                buff->lines[buff->y+buff->offset][strLength+3] = '\n';
                //buff->lines[y][strLength+1] = '\0';
                buff->x += 4;
              }
              else beep();
              break;
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_UP:
            case KEY_DOWN:
              buff = navigate(buff, editor, ch, 1);
              break;
          default: //any other char being entered
            if(strLength <= buff->COLS){
              for(int i = strLength; i >buff->x; --i){
                buff->lines[buff->y+buff->offset][i] = buff->lines[buff->y+buff->offset][i-1];
              }
              buff->lines[buff->y+buff->offset][buff->x] = ch;
              if(buff->x<buff->COLS)
                buff->x++;
            }
            else beep();
            break;
        }
        refreshBuff(editor,buff); wmove(editor,buff->y,buff->x);
        D(mvprintw(0,0," off%d R%d C%d L%d y%d x%d ",
            buff->offset, buff->ROWS, buff->COLS,buff->num_lines,buff->y,buff->x); refresh(););
    }
    return buff;
}
