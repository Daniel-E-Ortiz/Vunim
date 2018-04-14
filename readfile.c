#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "readfile.h"


// Reads from a file and returns an instance of the BUFFER struct as
//   it's return value
BUFFER *readfile(FILE *fp) {
  BUFFER *buff = malloc(sizeof(BUFFER));
  buff->num_lines = 0;
  buff->lines = calloc(buff->num_lines+1, sizeof(char *));
  buff->lines[0] = calloc(COLS+2, sizeof(char *));
  int i = 0;
  while(fgets(buff->lines[i], COLS+1, fp)) {
    //if((int)strlen(buff->lines[i])>COLS && buff->lines[i][COLS]!='\n') {
      beep();
    //}
    buff->num_lines++;
    buff->lines = realloc(buff->lines, (buff->num_lines+1)*sizeof(char *));
    buff->lines[++i] = calloc(COLS+2, sizeof(char *));
  }
  buff->lines[buff->num_lines] = NULL;

  return buff;
}

