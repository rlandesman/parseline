#include "parseline.h"

int main(int argc, char **argv){
	int stageCount = 0;
	int argCount = 0;
	char str[MAX_CMND_ARGS];
	char *tempStr;
	char *temp;
	int extra;
	char ch;
	int i;
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
	while(token != NULL){
		char *end_token;
		remove_newline_ch(token);

		/* ERROR CHECK */
		temp = getCommand(token);
		printLine(stageCount, token);

		char *token2 = strtok_r(token, " ", &end_token);

		/*instantiating array of structs */
		stageArray[stageCount].argc = 0;
		stageArray[stageCount].stageNum = stageCount;

		while(token2 != NULL){
			stageArray[stageCount].argc++;
			if(token2[0] == '<'){
				token2 = strtok_r(NULL, " ", &end_token);
				strcpy(stageArray[stageCount].input, token2);
			}
			if(token2[0] == '>'){
				token2 = strtok_r(NULL, " ", &end_token);
				strcpy(stageArray[stageCount].output,token2);
			}
			strcpy(stageArray[stageCount].argv[argCount],token);
			token2 = strtok_r(NULL, " ", &end_token);
			argCount++;
		}
		stageCount++;
		token = strtok_r(NULL, "|", &end_str);
	}

	for(i = 0; i < stageCount; i++){
		printf("OUTPUT: %s\n", stageArray[i].output);
		printf("INPUT: %s\n", stageArray[i].input);

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

char *getCommand(char *str){
	char *temp = malloc(sizeof(strlen(str)));
	int i;
	for(i = 0; i < strlen(str); i++){
		if(str[i] != ' '){
			temp[i] = str[i];
		} else {
			temp[i] = '\0';
			break;
		}

	}
	return temp;
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
				return 1;
			} else {
				curCmd = in->words[i];
				cmdargs++;

				if (++pipelen > MAX_PIPE_LEN) { /*check if pipe too long*/
					fprintf(stderr, "Pipeline too deep.\n");
					return 1;
				}
			}
		} else { /*same command*/
			switch (in->words[i][0]) {
				case '|': 
					if (outset == 2) { /*if out already set*/
						fprintf(stderr, "%s: Ambiguous output.\n", curCmd);
						return 1;
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