#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "insert.h"
#include "buffer.h"

BUFFER *insert(WINDOW *editor, BUFFER *buff){
  if(buff->num_lines>=buff->ROWS && buff->y==buff->ROWS-1) {
    buff->offset++; buff->y--;
  }
  buff->num_lines++;
	buff->lines = realloc(buff->lines, ((buff->num_lines+1) * sizeof(char *)));

	//Moves memory one over
	for(int i = buff->num_lines; i > (buff->y+buff->offset); i--)
		buff->lines[i] = buff->lines[i-1];

  //Allocates the space for the new string, makes it newline and null terminates
	buff->lines[buff->y+buff->offset] = calloc(COLS+2, sizeof(char *));
  buff->lines[buff->y+buff->offset][0] = '\n';
  buff->lines[buff->y+buff->offset][1] = '\0';

  return buff;
}
