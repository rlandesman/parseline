#include <stdio.h>

int main(int argc, char **argv){
	int bytes;
	int x = 0;
	for(; x < argc; x++){
		bytes += sizeof(argv[x]);
	}
	
	printf("Sizeof of argv: %d\n", bytes);
	printf("ARGC %d\n", argc);
}