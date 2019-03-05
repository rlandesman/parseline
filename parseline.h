#ifndef PARSELINE_H
#define PARSELINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CMND_ARGS 512
#define MAX_PIPE_LEN 10

void printLine(int, char *);
void remove_newline_ch(char *);
int checkDoubleAliOUT(char *str);
int checkDoubleAliIN(char *str);
int checkDoubleBars(char *str);
char *getCommand(char *str);
void checkAmibiousInput(char *str);






#endif