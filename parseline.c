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
	char* cmdstr;

	printf("line: ");
	fgets(str, MAX_CMND_ARGS, stdin);


	strncpy(str2,str, MAX_CMND_ARGS);
	input fullString;
	initInput(str2, &fullString);
	LineErrCheck(&fullString);


	char *end_str;
	char *token = strtok_r(str, "|", &end_str);
	stage stageArray[MAX_PIPE_LEN];
	initializeStages(stageArray);
	char *commands[MAX_PIPE_LEN];

	while(token != NULL){
		char *end_token;
		remove_newline_ch(token);
		cmdstr = strdup(token);
		commands[stageCount] = cmdstr;

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
	printf("%c", '\n');
	printf("%c", '\n');
	printStages(stageArray, stageCount, commands);
}

void printStages(stage stageArray[MAX_PIPE_LEN], int stageCount, char *commands[MAX_PIPE_LEN]){
	int i, p;
	for(i = 0; i < stageCount; i++){
		printLine(i, commands[i]);
		printf("%11s", "input: ");
		if (i == 0){
			printf("%s\n", stageArray[i].input);
		}
		else{
			printf("pipe from stage %d\n", i-1);
		}

		printf("%11s", "output: ");
		if ((i < stageCount - 1) && !strcmp(stageArray[i].output, "original stdout")){	
			printf("pipe to stage %d\n", i+1);
		}
		else{
			printf("%s\n", stageArray[i].output);
		}
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



void LineErrCheck(input *in) {
	int stages = 0;
	int argcs = 0;
	int AlligatorIn = 0; 
	int AlligatorOut = 0; 
	char * cmd;
	int i;

	for (i = 0; i < in->len; i++) {

		if (!argcs) {

			if (in->str[i][0] == '|') {
				fprintf(stderr, "invalid null command.\n"); 
				exit(1);
			}

			else if(in->str[i][0] == '<'){
				fprintf(stderr, "invalid null command.\n"); 
				exit(1);
			}

			else if (in->str[i][0] == '>'){
				fprintf(stderr, "invalid null command.\n"); 
				exit(1);
			}

			else {
				if (++stages > MAX_PIPE_LEN) { 
					fprintf(stderr, "pipeline too deep.\n");
					exit(1);
				}

			}

		cmd = in->str[i];
		argcs += 1;

		}

		else if (in->str[i][0] == '|'){
			if (AlligatorOut == 2){
				fprintf(stderr, "%s: ambiguous output.\n", cmd);
				exit(1);
			}
			else{
				AlligatorOut = 0;
				argcs = 0;
				AlligatorIn = 1;
			}
		}
		else if (in->str[i][0] == '<'){
			if (AlligatorIn == 1){
				fprintf(stderr, "%s: ambiguous input\n", cmd);
				exit(1);
			}
			else if (AlligatorIn == 2){
				fprintf(stderr, "%s: bad input redirection.\n", cmd);
				exit(1);
			}
			else{
				AlligatorIn = 2;
				if (in->str[i+1][0] == '|' || in->str[i+1][0] == '<' 
					|| in->str[i+1][0] == '>' || in->str[i+1][0] == '\0'){
					fprintf(stderr, "%s: bad input redirection.\n", cmd);
					exit(1);
				}
			}
		}

		else if (in->str[i][0] == '>'){
			if (AlligatorOut == 2) {
				fprintf(stderr, "%s: bad input redirection.\n", cmd);
				exit(1);
			} 
			else {
				AlligatorOut = 2;
				if (in->str[i+1][0] == '|'){
					fprintf(stderr, "%s: bad input redirection.\n", cmd);
					exit(1);

				} 
				if (in->str[i+1][0] == '<'){
					fprintf(stderr, "%s: bad input redirection.\n", cmd);
					exit(1);
				} 
				if(in->str[i+1][0] == '>'){
					fprintf(stderr, "%s: bad input redirection.\n", cmd);
					exit(1);
				} 
				if (in->str[i+1][0] == '\0') {
					fprintf(stderr, "%s: bad input redirection.\n", cmd);
					exit(1);
				}
			
		else {
			if (++argcs > MAX_CMND_NUM) { 
				fprintf(stderr, "%s: Too many arguments.\n", cmd);
				exit(1);
			}
		}	
	}
	}
	}
}



void initInput(char *str, input *in) {
	in->len = 0;
	in->str[in->len++] = strtok(str, " \n");

	while ((in->str[in->len++] = strtok(NULL, " \n")) != NULL) {
		if (in->len == (10 * (10 + 3) + 1) 
			&& in->str[in->len - 1] != NULL) {
			break;
		}
	}

	in->len--; 
}