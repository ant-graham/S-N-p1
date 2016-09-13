# S-N-p1


#include "parse.h"
#include "myshell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



/*************************************************************************
*	Gets command from user and puts in buffers
*	(I used three separate buffers so I could strtok() each).
*************************************************************************/

void getCommand(char *buffer, char *exitBuffer, char *loopBuffer)
{
	fprintf(stdout, "[usr@myshell]: ");
	fgets(buffer, 256, stdin);
	strcpy(exitBuffer, buffer);
	strcpy(loopBuffer, buffer);
}



/*************************************************************************
*	Gets first token in buffer.
*************************************************************************/

void getFirstToken(Param_t *param, char *buffer, char *tokenPtr, char *delim, State_t *state)
{
	tokenPtr = strtok(buffer, delim);

	if(tokenPtr == NULL)  return;
	else if((*tokenPtr == '<') || (*tokenPtr == '>'))  *state = error;
	else param->argumentVector[param->argVectorCount++] = tokenPtr;
}



/*************************************************************************
*	Initializes all members of struct param.
*************************************************************************/

void initStructParam(Param_t *param)
{
	int i;
	param->inputRedirect = NULL;
	param->outputRedirect = NULL;
	param->background = 0;
	param->argVectorCount = 0;
	for(i = 0; i < MAXARGS; i++)
		param->argumentVector[i] = NULL;
}	



/*************************************************************************
*	Sets the initial state of the state machine.
*************************************************************************/	

State_t initState(char *tokenPtr, State_t *state, int *i)
{
	if(! *i){
		if(*tokenPtr == '<')       *state = input;
		else if(*tokenPtr == '>')  *state = output;
		else					   *state = normal;
		(*i)++;
	}
	return *state;
}



/*************************************************************************
*	Parses user-entered command and checks for illegal input.
*************************************************************************/

void parseInput(Param_t *param, char *buffer, char *tokenPtr, char *delim, State_t *state)
{
	int i = 0, j = 1;

	getFirstToken(param, buffer, tokenPtr, delim, state);

	while(((tokenPtr = strtok(NULL, delim)) != NULL) && (j++ < MAXARGS)){
		switch(initState(tokenPtr, state, &i)){
			case 0: stateNormal(param, tokenPtr, state);  break;
			case 1: stateInput(param, tokenPtr, state);   break;
			case 2: stateOutput(param, tokenPtr, state);  break;
			case 3: statePostIn(param, tokenPtr, state);  break;
			case 4: statePostOut(param, tokenPtr, state); break;
			case 5: statePostIO(param, tokenPtr, state);  break;
			case 6: stateEnd(tokenPtr, state);            break;
			default:						              break;
		}
		if(*state == error)  printf("myshell: command not found\n");  
	}
}



/*************************************************************************
*	Handles token as appropriate for normal state.
*************************************************************************/

void stateNormal(Param_t *param, char *tokenPtr, State_t *state) 
{
	if(*tokenPtr == '&'){ 
		param->background = 1; 
		*state = end;
	}
	else if(*tokenPtr == '<')  stateInput(param, tokenPtr, state);
	else if(*tokenPtr == '>')  stateOutput(param, tokenPtr, state);
	else  param->argumentVector[param->argVectorCount++] = tokenPtr;
}



/*************************************************************************
*	Handles token as appropriate for input state.
*************************************************************************/

void stateInput(Param_t *param, char *tokenPtr, State_t *state) 
{
	if(*(tokenPtr + 1) == ' '){
		*state = error;
		return;
	}
	param->inputRedirect = (tokenPtr + 1);
	*state = postInput;
}



/*************************************************************************
*	Handles token as appropriate for output state.
*************************************************************************/

void stateOutput(Param_t *param, char *tokenPtr, State_t *state) 
{
	if(*(tokenPtr + 1) == ' '){
		*state = error;
		return;
	}
	param->outputRedirect = (tokenPtr + 1);
	*state = postOutput;
}



/*************************************************************************
*	Handles token as appropriate for post input state.
*************************************************************************/

void statePostIn(Param_t *param, char *tokenPtr, State_t *state) 
{
	if((*tokenPtr != '>') && (*tokenPtr != '&'))  *state = error; 
	else if(*tokenPtr == '>'){
		param->outputRedirect = (tokenPtr + 1);
		*state = postIO;
	}
	else if(*tokenPtr == '&'){
		param->background = 1;
		*state = end;
	}
}



/*************************************************************************
*	Handles token as appropriate for post output state.
*************************************************************************/

void statePostOut(Param_t *param, char *tokenPtr, State_t *state) 
{
	if((*tokenPtr != '<') && (*tokenPtr != '&')) *state = error;
	else if(*tokenPtr == '<'){
		param->inputRedirect = (tokenPtr + 1);
		*state = postIO;
	}
	else if(*tokenPtr == '&'){
		param->background = 1;
		*state = end;
	}
}



/*************************************************************************
*	Handles token as appropriate for post IO state.
*************************************************************************/

void statePostIO(Param_t *param, char *tokenPtr, State_t *state) 
{
	if(*tokenPtr == '&'){
		param->background = 1;
		*state = end;
	}
	else *state = error;
}



/*************************************************************************
*	Handles token as appropriate for end state.
*************************************************************************/

void stateEnd(char *tokenPtr, State_t *state)
{
	if(tokenPtr != NULL)  *state = error;
}



/*************************************************************************
*	Prints parameters if in Debug mode.	
*************************************************************************/

void printParams(Param_t *param)
{
	int i;
	printf("InputRedirect: [%s]\n", (param->inputRedirect != NULL) ? param->inputRedirect : "NULL");
	printf("OutputRedirect: [%s]\n", (param->outputRedirect != NULL) ? param->outputRedirect : "NULL");
	printf("Background: [%d]\n", param->background);
	printf("ArgumentCount: [%d]\n", param->argVectorCount);
	for(i = 0; i < param->argVectorCount; i++)
		printf("ArgumentVector[%2d]: [%s]\n", i, param->argumentVector[i]);
}
