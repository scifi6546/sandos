#include "libsandos.h"
#include <stdio.h>
#include "gui.h"
int main(int argc, char *argv[]){
	sandbox(argv[1],argv[2]);
	puts("hi there");
}
