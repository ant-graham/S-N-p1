# S-N-p1


#include "myshell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
	Param_t param;
	State_t state;
	char buffer[256], exitBuffer[256], loopBuffer[256], *tokenPtr = NULL, *delim = " \n\t|";
	pid_t cpid, status;
	FILE *cpidTracker = NULL;

	if(argv[1] != NULL && strcmp(argv[1], "-Debug") != 0)
		fprintf(stderr, "%s is an invalid command.  Unable to start program.\n", argv[1]);

	while(1){
		initStructParam(&param);
		getCommand(buffer, exitBuffer, loopBuffer);
		if((tokenPtr = strtok(loopBuffer, delim)) == NULL)  continue;
		//exit was here
		parseInput(&param, buffer, tokenPtr, delim, &state);
		executeShell(&param, &cpid, &status, cpidTracker);
		exitProg(exitBuffer, delim, &status, cpidTracker);
		if(argv[1] && (state != error))  printParams(&param);
	}	

	return 0;
}



/*************************************************************************
*	Exits program 
*************************************************************************/

void exitProg(char *exitBuffer, char *delim, pid_t *status, FILE *cpidTracker)
{
	char *tokenPtr = strtok(exitBuffer, delim);
	pid_t cpid;
	cpidTracker = fopen("cpid.txt", "r");
	
	if((strcmp(tokenPtr, "exit") == 0) || (strcmp(tokenPtr, "Exit") == 0)){
		while(fscanf(cpidTracker, "%d", &cpid) != EOF)
			waitpid(cpid, status, 0);
			
		fclose(cpidTracker);
		exit(0);
	}
}



/*************************************************************************
*	Executes shell
*************************************************************************/

void executeShell(Param_t *param, pid_t *cpid, pid_t *status, FILE *cpidTracker)
{
	char *in = NULL, *out = NULL;
	cpidTracker = fopen("cpid.txt", "w");

	if((*cpid = fork()) < 0)  fprintf(stderr, "Fork failure.\n");
	else if(*cpid > 0){
		if(! param->background)  waitpid(*cpid, status, 0);
		else{
			cpidTracker = fopen("cpid.txt", "a");
			fprintf(cpidTracker, "%d\n", *cpid);
		}
	} else{
		if((in = param->inputRedirect) != NULL)    freopen(in, "r", stdin);
		if((out = param->outputRedirect) != NULL)  freopen(out, "w", stdout);

		if((execvp(param->argumentVector[0], param->argumentVector) < 0) &&
			((strcmp(param->argumentVector[0], "Exit") != 0) &&
			 (strcmp(param->argumentVector[0], "exit") != 0))){
			fprintf(stderr, "%s: command not found\n", param->argumentVector[0]);
			exit(1);
		}
	}
}		
