#include <ncurses.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "searchreplace.h"
#include "buffer.h"

BUFFER *search_replace(BUFFER *buff,WINDOW *cmd, WINDOW *editor){
  char *search = (char *)calloc(100,sizeof(char));
  char *replace = (char *)calloc(100,sizeof(char));
  char searchPrompt[] = "Search: ";
  char replacePrompt[] = "Replace: ";
  char resultsPrompt[] = "Results: | Replaced \"%s\" with \"%s\" successfully! |";
  char errorPrompt[] = "Error: replacing \"%s\" with \"%s\" is too big for line \"%d\"";
  wclear(cmd);
  wprintw(cmd,searchPrompt);
  wgetstr(cmd,search);
  wclear(cmd);
  wprintw(cmd,replacePrompt);
  wgetstr(cmd,replace);
  int lines = 0;
  int count = 0;
  int y = 0;
  while(lines < buff->num_lines){
    char *subString = (char *)malloc(sizeof(buff->lines[y]));
    while((subString = strstr(subString,search))){
      count++;
      subString++;
    }
    if(((int)strlen(buff->lines[y])+(count*(int)strlen(replace))+1) > buff->COLS){
      wclear(cmd);
      wprintw(cmd,errorPrompt,search,replace,y+1);
      wrefresh(cmd);
    }
    else{
      subString = buff->lines[y];
      while((subString = strstr(subString,search))){
        memmove(subString+strlen(replace),subString+strlen(search),strlen(subString));
        strncpy(subString,replace,strlen(replace));
      }
    }
    y++;
    lines++;
    free(subString);
  }
  wclear(cmd);
  wprintw(cmd,resultsPrompt,search,replace);
  wrefresh(cmd);
  buff->x=0; buff->y=0;
  return buff;
}
