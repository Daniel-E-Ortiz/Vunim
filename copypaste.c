#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "copypaste.h"

BUFFER* copy(WINDOW* editor, int length, BUFFER *buff)
{
    //checks to make sure the user put in a valid number of characters
    //to copy
    if(length<=0) return NULL;

    //frees any current copied string
	if(buff->copybuff) { free(buff->copybuff); buff->copybuff = NULL; }

    //allocates memory for new copied string
	buff->copybuff = malloc(length*sizeof(char)+1);

    //makes sure user isn't trying to copy more than is in the line
	if ((buff->x+length)>COLS || (buff->x+length)>=(int)strlen(buff->lines[buff->y+buff->offset]))
	{
		length = (int)strlen(buff->lines[buff->y+buff->offset])-1-buff->x;
	}

    //copies characters into copybuff
	winnstr(editor, buff->copybuff, length);
	return buff;
}

void paste(WINDOW* editor, BUFFER *buff)
{
    //ensures a valid string has been passed to the function
    if(!buff->copybuff) return;

    //gets *string length and the current line length
    int inslen = (int)strlen(buff->copybuff);
    int curlen = (int)strlen(buff->lines[buff->y+buff->offset])-1;

    //ensures user is not inserting beyond the end of the
    //allotted space for a line
    if(inslen+curlen > COLS) return;

    //moves characters after current x to the right by inslen spaces
    for(int i = curlen+1; i>=buff->x; i--)
        buff->lines[buff->y+buff->offset][i+inslen] = buff->lines[buff->y+buff->offset][i];

    int cpindex = 0;

    //inserts string into line
    for(int i = 0; i < inslen; i++) {
        buff->lines[buff->y+buff->offset][i+buff->x] = buff->copybuff[cpindex];
        cpindex++;
    }
}














