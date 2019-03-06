#include "parseline.h"

int main(int argc, char **argv){
	int stageCount = 0;
	int argCount = 0;
	char str[MAX_CMND_ARGS];
	char *tempStr;
	char *temp;
	char *tokenDup;
	int extra;
	char ch;
	int i, p, k;
	char str2[MAX_CMND_ARGS];

	printf("line: ");
	fgets(str, MAX_CMND_ARGS, stdin);

	strncpy(str2,str, MAX_CMND_ARGS);
	input fullString;
	initInput(str2, &fullString);
	inputErrorCheck(&fullString);


	char *end_str;
	char *token = strtok_r(str, "|", &end_str);
	stage stageArray[MAX_PIPE_LEN];
	initializeStages(stageArray);
	char *commands[MAX_PIPE_LEN];

	while(token != NULL){
		char *end_token;
		remove_newline_ch(token);
		commands[stageCount] = token;

		/* printLine(stageCount, token); */
		char *token2 = strtok_r(token, " ", &end_token);

		/*instantiating array of structs */
		stageArray[stageCount].argc = 0;
		stageArray[stageCount].stageNum = stageCount;

		while(token2 != NULL){
			stageArray[stageCount].argc = stageArray[stageCount].argc + 1;
			if(token2[0] == '<' || token2[1] == '<'){
				stageArray[stageCount].argc = stageArray[stageCount].argc - 1;
				token2 = strtok_r(NULL, " ", &end_token);
				strcpy(stageArray[stageCount].input, token2);
			} 
			else if(token2[0] == '>' || token2[1] == '>'){
				stageArray[stageCount].argc = stageArray[stageCount].argc - 1;
				token2 = strtok_r(NULL, " ", &end_token);
				strcpy(stageArray[stageCount].output,token2);
			} else {
				tokenDup = strdup(token2);
				strcpy(stageArray[stageCount].argv[argCount],tokenDup);
				argCount++;

			}

			token2 = strtok_r(NULL, " ", &end_token);
		}
		argCount = 0;
		stageCount++;
		token = strtok_r(NULL, "|", &end_str);
	}
	printStages(stageArray, stageCount, commands);
}

void printStages(stage stageArray[MAX_PIPE_LEN], int stageCount, char *commands[MAX_PIPE_LEN]){
	int i, p;
	for(i = 0; i < stageCount; i++){
		printLine(i, commands[i]);
		printf("     input: %s\n", stageArray[i].input);
		printf("     output: %s\n", stageArray[i].output);
		printf("     argc: %d\n", stageArray[i].argc);
		printf("     argv: ");
		for(p = 0; p < stageArray[i].argc - 1; p++){
			printf("%s, ", stageArray[i].argv[p]); /*in quotations */
		}
		if(stageArray[i].argc == 1){
		 	printf("%s\n",stageArray[i].argv[0]);
		} else {
			printf("%s\n",stageArray[i].argv[p]);
		}
	}
}

void printLine(int stage, char *argument){
	printf("--------\n");
	printf("Stage %d: ", stage);
	printf("%c", '"');
	printf("%s", argument);
	printf("%c\n", '"');
	printf("--------\n");
}

void initializeStages(stage stageArray[MAX_PIPE_LEN]){
	int x;
	for(x = 0; x < MAX_PIPE_LEN; x++){
		strcpy(stageArray[x].input, "original stdin");
		strcpy(stageArray[x].output, "original stdout");
	}
}

void remove_newline_ch(char *line)
{
    int new_line = strlen(line) - 1;
    if(strcmp(&line[0]," ")){ /*TODO: THIS IS SHIT */
    	line++;
    } 
    if(strcmp(&line[new_line - 2], " ")){
    	line[new_line - 1] = '\0';
    }

    if (line[new_line] == '\n')
        line[new_line] = '\0';
}

int inputErrorCheck(input *in) {
	int pipelen = 0;
	int cmdargs = 0;
	int inset = 0; 
	int outset = 0; 
	char *curCmd;
	int i;

	for (i = 0; i < in->size; i++) {
		if (cmdargs == 0) { /*new command*/
			if (in->words[i][0] == '|' || in->words[i][0] == '<' || in->words[i][0] == '>') {
				fprintf(stderr, "Invalid null command.\n"); 
				exit(1);
			} else {
				curCmd = in->words[i];
				cmdargs++;

				if (++pipelen > MAX_PIPE_LEN) { /*check if pipe too long*/
					fprintf(stderr, "Pipeline too deep.\n");
					exit(1);
				}
			}
		} else { /*same command*/
			switch (in->words[i][0]) {
				case '|': 
					if (outset == 2) { /*if out already set*/
						fprintf(stderr, "%s: Ambiguous output.\n", curCmd);
						exit(1);
					} else { /*reset cmdargs if new cmd coming*/
						cmdargs = 0;
						inset = 1;
						outset = 0;
					}
					break;
				case '<':
					if (inset == 1) { /*if in already set by pipe*/
						fprintf(stderr, "%s: Ambiguous input\n", curCmd);
						exit(1);
					} else if (inset == 2) { /*if in already set by redir*/
						fprintf(stderr, "%s: Bad input redirection.\n", curCmd);
						exit(1);
					} else { /*if not already set*/
						inset = 2;
						if (in->words[i+1][0] == '|' || in->words[i+1][0] == '<' /* if name is missing*/
							|| in->words[i+1][0] == '>' || in->words[i+1][0] == '\0') {
							fprintf(stderr, "%s: Bad input redirection.\n", curCmd);
							exit(1);
						}
					}
					break;
				case '>': 
					if (outset == 2) { /*if out already set by redir*/
						fprintf(stderr, "%s: Bad input redirection.\n", curCmd);
						exit(1);
					} else { /*if not already set*/
						outset = 2;
						if (in->words[i+1][0] == '|' || in->words[i+1][0] == '<' /* if name is missing*/
							|| in->words[i+1][0] == '>' || in->words[i+1][0] == '\0') {
							fprintf(stderr, "%s: Bad input redirection.\n", curCmd);
							exit(1);
						}
					}
					break;
				default:
					if (++cmdargs > MAX_CMND_NUM) { /*check if args too long*/
						fprintf(stderr, "%s: Too many arguments.\n", curCmd);
						exit(1);
					}
			}

		}
	}
	return 0;
}

void initInput(char *str, input *in) {
	in->size = 0;
	in->words[in->size++] = strtok(str, " \n");

	while ((in->words[in->size++] = strtok(NULL, " \n"))) {
		if (in->size == (10 * (10 + 3) + 1) 
			&& in->words[in->size - 1] != NULL) {
			break;
		}
	}

	in->size--; 
}