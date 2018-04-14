#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "delete.h"
#include "buffer.h"

BUFFER *delete(WINDOW *editor, BUFFER *buff){
	//Free's the pointer to be deleted later.
	free(buff->lines[buff->y+buff->offset]);

	//Moves everything in the array up one, to get rid of the deleted line.
	for(int i = buff->y+buff->offset; i <= buff->num_lines; i++)
		buff->lines[i] = buff->lines[i+1];
	
	//Decreases the number of lines in the BUFFER struct. 
	buff->num_lines--;
	
	//Reallocates the array to one less than before. 
	buff->lines = realloc(buff->lines, ((buff->num_lines + 1) * sizeof(char *)));
	
	//Refreshes the buffer. 
	//refreshBuff(editor, buff);
  if(buff->offset!=0)
    buff->offset--; 
	return buff;

}
