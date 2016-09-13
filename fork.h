# S-N-p1



#ifndef FORK_H
#define FORK_H

// Forks and has the parent wait for the children to exit

void forkit(char *parent, int instance, int times);


// Creates a child process
void createchild();
#endif /* FORK_H */

