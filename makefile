make: main.c
	gcc -o out main.c
debug: main.c
	gcc -g3 -O0 -C -o debug main.c
valgrind: main.c
	gcc -g -o valgrind main.c
