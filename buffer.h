#ifndef BUFFER_H
#define BUFFER_H

typedef struct BUFFER {
  char **lines;
  int num_lines;
  int y, x, offset;
  char* copybuff;
  int ROWS, COLS;
}BUFFER;

void refreshBuff(WINDOW *editor, BUFFER *buff);

#endif
