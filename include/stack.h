#ifndef STACK_H
#define STACK_H

#include "song.h"

typedef struct Stack {
    Song *top;
} Stack;

void initStack(Stack *stack);
int isStackEmpty(const Stack *stack);
void pushSong(Stack *stack, const Song *song);
Song *popSong(Stack *stack);
Song *peekStack(const Stack *stack);
void showStack(const Stack *stack);
void clearStack(Stack *stack);

#endif
