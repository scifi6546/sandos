CC=gcc
CFLAGS=-Wall 'pkg-config --libs --cflags gtk+-3.0'
DEBUG=-g3
VALGRIND=-g
OBJ = main.o libsandos.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
