#include "libsandos.h"
#include <stdio.h>
#include "gui.h"
int main(int argc, char *argv[]){
	if(argc==3){
		sandbox(argv[1],argv[2]);
	}
	if(argc==4){
		remove_sandbox(argv[1],argv[2]);

	}
	return 0;
}
