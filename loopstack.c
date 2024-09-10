#include "loopstack.h"


struct LoopStack* createLoop(char* value)
{
    struct LoopStack* newNode = malloc(sizeof(struct LoopStack));
    newNode->loopStart = value;

    return newNode;
}

struct LoopStack* createRootNode()
{
    struct LoopStack* rootNode = malloc(sizeof(struct LoopStack));

    // this value never changes; all loops are pushed on top of this root node
    rootNode->loopStart = NULL;
    return rootNode;
}

struct LoopStack* pushLoop(struct LoopStack* root, char* value)
{
    struct LoopStack* newNode = createLoop(value);

    newNode->next = root;
    newNode->loopStart = value;

    return newNode;
}

char* popLoop(struct LoopStack* root)
{
    char* loopStart = root->loopStart;

    // handle root node being mistakenly passed to function
    if (root->next == NULL)
    {
        return NULL;
    }

    *root = *root->next;
    return loopStart;
}

void freeLoopStack(struct LoopStack* root)
{
    if (root->next == NULL)
        free(root);
    else
    {
        struct LoopStack* next = root->next;
        free(root);
        freeLoopStack(next);
    }
}
