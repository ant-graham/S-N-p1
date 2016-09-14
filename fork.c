# S-N-p1
#include "myshell.h"
#include "fork.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



void forkit(char* argv ,int instances, int times)
{
    int i = 0;
    pid_t parent = getpid();
    
    for(i;i < instances;i++)
    {
        pid_t child = fork();
        
        //Failed to fork
        if (child == -1)
        {
            printf("Unable to fork process\n");
            exit(1);
        }
        //Wait for child processes to finish
        else if (child > 0)
        {
            int status;
            
            waitpid(child, &status, 0);
        }
        else
        {
            // NO ZOMBIES
            execvp(argv[0],argv);
            exit(1);
        }
    }
    
}
