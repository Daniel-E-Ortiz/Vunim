#include <curses.h>
#include <stdio.h>
#include "save.h"

void save_to_file(BUFFER *buff, const char *file){
    FILE *fp = fopen(file, "w");
    
    int ch, y=0, x=0, lines=0;
    
    while(lines < buff->num_lines){
        fputs(buff->lines[y], fp);
        
        y++;
        lines++;
    }
    
    fclose(fp);
}
