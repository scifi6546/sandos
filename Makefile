CC=gcc
CFLAGS=-Wall
DEBUG=-g3
VALGRIND=-g
OBJ = main.o libsandos.o
DEPS = libsandos.h

%.o: %.c $(DEPS)
	$(CC) -c $(DEBUG) -o $@ $< $(CFLAGS)

main: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
