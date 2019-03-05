/*
ERROR CHECKS: Double Bar, 512 Byte input 
*/

#include "parseline.h"

int main(int argc, char **argv){
	int stageCount = 0;
	int argCount = 0;
	char str[MAX_CMND_ARGS];
	char *tempStr;
	char *temp;

	printf("line: ");
	fgets(str, MAX_CMND_ARGS, stdin);
	if(checkDoubleBars(str) == -1){
		printf("Invalid null command\n");
		exit(1);
	}
	checkAmibiousInput(str);

	char *end_str;
	char *token = strtok_r(str, "|", &end_str);

	while(token != NULL){
		char *end_token;
		remove_newline_ch(token);

		/* ERROR CHECK */
		temp = getCommand(token);

		if(checkDoubleAliIN(token) == -1){
			printf("%s: Bad Input redirection\n", temp);
			exit(1);	
		}

		if(checkDoubleAliOUT(token) == -1){
			printf("%s: Bad output redirection\n", temp);
			exit(1);	
		}
		printLine(stageCount, token);

		char *token2 = strtok_r(token, " ", &end_token);
		while(token2 != NULL){
			token2 = strtok_r(NULL, " ", &end_token);
			argCount++;
		}
		stageCount++;
		token = strtok_r(NULL, "|", &end_str);
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

int checkDoubleBars(char *str){
	int i = 0;
	for(; i < strlen(str); i++){
		if(str[i] == '|'){
			if(str[i-2] == '|'){
				return -1;
			}
		}
	}
	return 0;
}

int checkDoubleAliIN(char *str){
	int cnt = 0;
	int i = 0;
	for(; i < strlen(str); i++){
		if(str[i] == '<'){
			cnt++;
		}
	}
	if(cnt > 1){
		return -1;
	} else {
		return 0;
	}
}

int checkDoubleAliOUT(char *str){
	int cnt = 0;
	int i = 0;
	for(; i < strlen(str); i++){
		if(str[i] == '>'){
			cnt++;
		}
	}
	if(cnt > 1){
		return -1;
	} else {
		return 0;
	}
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

void checkAmibiousInput(char *str){
	int i;
	int flag = 0;
	char *temp = strdup(str);
	char *word;
	word = strrchr(temp, '|');
	char *tok = strtok(word, " ");
	tok = strtok(NULL," ");

	for(i = 0; i < strlen(str); i++){
		if(str[i] == '<'){
			flag = 1;
		}
		if(str[i] == '|'){
			flag = 0;
		}
		if(flag == 1){
			printf("%s: Amibgious Input\n", tok);
			exit(1);
		}
	}	
	
}