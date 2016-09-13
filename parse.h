# S-N-p1

#ifndef _parse_h
#define _parse_h

//Defines constant for maximum arguments
#define MAXARGS 32

//Structure to hold input data
struct param
{
    char *inputRedirect;
    char *outputRedirect;
    int background;
    int argVectorCount;
    char *argumentVector[MAXARGS];
};



//A typedef for faster access to struct param
typedef struct param Param_t;

//A typedef for an enumerated data type for state machine
typedef enum {normal, input, output, postInput, postOutput, postIO, end, error} State_t;



//This function prints the contents of struct param.
//Parameter: param
void printParams(Param_t *param);

//This function initializes all pointers in struct param to null and all int
//values to zero
//Parameter: param
void initStructParam(Param_t *param);

//This function displays a prompt, the gets input from user and stores it in
//buffer.
//Parameter: buffer
void getCommand(char *buffer, char *exitBuffer, char *loopBuffer);

//This function gets first token of user entered command line.  
//If input or output redirect chars are entered, state turns to error
//which will override normal shell behavior, per project details in
//meta language
//Parameters: param, buffer, tokenPtr, delim, state
void getFirstToken(Param_t *param, char *buffer, char *tokenPtr, char *delim, State_t *state);

//This function sets the initial state of state machine, then simply returns
//the state set within the state machine.  Return type is enum type defined in
//parse.h
//Parameters: tokenPtr, state, i, debug 
State_t initState(char *tokenPtr, State_t *state, int *i);

//This function calls state functions depending on what the current state of the 
//state machine is.
//Parameters: param, tokenPtr, delim, state
void parseInput(Param_t *param, char *buffer, char *tokenPtr, char *delim, State_t *state);

//This function performs the appropriate operations on a token that belongs
//in the argVector.  Will also send args to input or output state if necessary.
//Parameters: param, tokenPtr, state
void stateNormal(Param_t *param, char *tokenPtr, State_t *state);

//This function handles a token that is to be an input file.  Changes state
//to postIn when through.
//Parameters: param, tokenPtr, state
void stateInput(Param_t *param, char *tokenPtr, State_t *state);

//This function handles a token that is to be an output file.  Changes state
//to postOut when through.
//Parameters: param, tokenPtr, state
void stateOutput(Param_t *param, char *tokenPtr, State_t *state);

//This function handles tokens on command line after an input file is entered
//to be sure legal tokens are entered.  If not, state is set to error.
//Parameters: param, tokenPtr, state
void statePostIn(Param_t *param, char *tokenPtr, State_t *state);

//This function handles tokens on command line after an output file is entered
//to be sure legal tokens are entered.  If not, state is set to error.
//Parameters: param, tokenPtr, state
void statePostOut(Param_t *param, char *tokenPtr, State_t *state);

//This function handles tokens on command line after both input and output files
//have been entered.  Makes sure tokens are legal.  If not, state is set to error.
//Parameters: param, tokenPtr, state
void statePostIO(Param_t *param, char *tokenPtr, State_t *state);

//This function makes sure there are no more tokens entered after ampersand on 
//command line.  If there are, state is set to error.
//Parameters: tokenPtr, state
void stateEnd(char *tokenPtr, State_t *state);

#endif
