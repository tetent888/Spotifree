/*
 * stack.c - History stack for previously played songs
 */

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

void stackInit(Stack *stack) {
    stack->top = NULL;
}

int stackIsEmpty(Stack *stack) {
    return stack->top == NULL;
}

void stackPush(Stack *stack, const Song *song) {
    Song *newSong = createSong(song);

    if (newSong == NULL) {
        return;
    }

    newSong->next = stack->top;
    stack->top = newSong;
}

Song *stackPop(Stack *stack) {
    Song *song;

    if (stackIsEmpty(stack)) {
        return NULL;
    }

    song = stack->top;
    stack->top = stack->top->next;
    song->next = NULL;

    return song;
}

Song *stackPeek(Stack *stack) {
    return stack->top;
}

void stackShow(Stack *stack) {
    Song *curr = stack->top;
    int number = 1;

    if (stackIsEmpty(stack)) {
        printf("(History is empty)\n");
        return;
    }

    printf("\n========== HISTORY ==========\n");
    while (curr != NULL) {
        printf("%d. [%s] %s - %s (%d:%02d)\n",
               number, curr->id, curr->title, curr->artist,
               curr->duration_seconds / 60, curr->duration_seconds % 60);
        curr = curr->next;
        number++;
    }
    printf("=============================\n\n");
}

void stackClear(Stack *stack) {
    Song *curr = stack->top;

    while (curr != NULL) {
        Song *temp = curr;
        curr = curr->next;
        free(temp);
    }

    stack->top = NULL;
}
