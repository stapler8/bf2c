#ifndef BF2C_LOOPSTACK_H
#define BF2C_LOOPSTACK_H


#include <stdlib.h>


typedef struct LoopStack
{
    char* loopStart;
    struct LoopStack* next;
} LoopStack;

struct LoopStack* createLoop(char*);
struct LoopStack* createRootNode();
struct LoopStack* pushLoop(struct LoopStack*, char*);
char* popLoop(struct LoopStack*);
void freeLoopStack(struct LoopStack*);


#endif //BF2C_LOOPSTACK_H
