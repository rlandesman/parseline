#include "parseline.h"

int main(int argc, char **argv){
	char *command;
	int x = 0;
	int byteLength;
	int commandArgs;
	char str[MAX_CMND_ARGS];

	printf("line: ");
	/* Check limitations, see README */
	if(argc < 2){
		printf("Error: command line arguments not supplied\n");
		exit(1);
	}

	fgets(str, MAX_CMND_ARGS, stdin);

}