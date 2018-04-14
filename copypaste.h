#ifndef COPYPASTE_H
#define COPYPASTE_H
#include "buffer.h"

BUFFER* copy(WINDOW *editor, int length, BUFFER *buff);
void paste(WINDOW *editor, BUFFER *buff);

#endif

