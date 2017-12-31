CC=gcc
CFLAGS=-Wall
DEBUG=-g3 -C
VALGRIND=-g
OBJ = main.o libsandos.o
DEPS = libsandos.h

%.o: %.c $(DEPS)
	$(CC) -c $(DEBUG) -o $@ $< $(CFLAGS)

main: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
