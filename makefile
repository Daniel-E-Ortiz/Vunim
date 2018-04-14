# Makefile for project2 unix: vunim (V unimproved)

OBJ = readfile.o copypaste.o mode.o save.o edit.o insert.o delete.o \
      searchreplace.o buffer.o nav.o
CFLAGS = -c -std=c99

ifeq ($(debug), 1)
	CFLAGS += -g -DDEBUG
endif

vunim: main.o $(OBJ)
	gcc -o $@ main.o $(OBJ) -lncurses

main.o: main.c buffer.h debug.h
	gcc $(CFLAGS) $< -o $@

$(OBJ): %.o: %.c %.h buffer.h
	gcc $(CFLAGS) $< -o $@

edit.o: insert.h copypaste.h delete.h nav.h debug.h

.PHONY: clean
clean:
	rm -f vunim $(OBJ) main.o
