# S-N-p1

#ifndef _myshell_h
#define _myshell_h

#include "parse.h"
#include <sys/types.h>
#include <stdio.h>

//This function exits program if first token in buffer is "exit"
//Parameters: exitBuffer, delim
void exitProg(char *exitBuffer, char *delim, pid_t *status, FILE *cpidTracker);

//This function executes shell by creating child processes and 
//calling execvp() to execute a program
//Parameters: param, cpid, status, children
void executeShell(Param_t *param, pid_t *cpid, pid_t *status, FILE *cpidTracker);

#endif
