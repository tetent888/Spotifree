#ifndef STACK_H
#define STACK_H

#include "song.h"

typedef struct Stack {
    Song *top;
} Stack;

void stackInit(Stack *stack);
int stackIsEmpty(Stack *stack);
void stackPush(Stack *stack, const Song *song);
Song *stackPop(Stack *stack);
Song *stackPeek(Stack *stack);
void stackShow(Stack *stack);
void stackClear(Stack *stack);

#endif
