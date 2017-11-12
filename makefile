make: main.c
	gcc -o out main.c
debug: main.c
	gcc -g3 -o debug main.c
valgrind: main.c
	gcc -g -o valgrind main.c
