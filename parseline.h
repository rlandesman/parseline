#ifndef PARSELINE_H
#define PARSELINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CMND_ARGS 512
#define MAX_PIPE_LEN 10
#define MAX_CMND_NUM 10
#define ARGC_MAX 10

typedef struct stage{
	int argc;
	char argv[MAX_CMND_ARGS][ARGC_MAX];
	char input[MAX_CMND_ARGS];
	char output[MAX_CMND_ARGS];
	int stageNum;
} stage;

typedef struct input {
	char *words[MAX_CMND_ARGS]; /*3 extra args for name, <, >*/
	int size;
} input;

void printLine(int, char *);
void remove_newline_ch(char *);
char *getCommand(char *str);
int inputErrorCheck(input *in);
void initInput(char *str, input *in);








#endif