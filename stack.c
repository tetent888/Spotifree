/*
 * stack.c - Stack data structure for play history
 */

#include <stdio.h>
#include <stdlib.h>

#include "include/stack.h"

void initStack(Stack *stack) {
    stack->top = NULL;
}

int isStackEmpty(const Stack *stack) {
    return stack->top == NULL;
}

void pushSong(Stack *stack, const Song *song) {
    if (song == NULL) {
        printf("[stack] Cannot push an empty song.\n");
        return;
    }

    Song *newSong = createSong(song->id, song->title, song->artist, song->duration);
    if (newSong == NULL) return;

    newSong->next = stack->top;
    stack->top = newSong;

    printf("[stack] Pushed \"%s\" to history.\n", song->title);
}

Song *popSong(Stack *stack) {
    if (isStackEmpty(stack)) {
        printf("[stack] History stack is empty.\n");
        return NULL;
    }

    Song *song = stack->top;
    stack->top = stack->top->next;
    song->next = NULL;

    return song;
}

Song *peekStack(const Stack *stack) {
    return stack->top;
}

void showStack(const Stack *stack) {
    if (isStackEmpty(stack)) {
        printf("  (Stack is empty)\n");
        return;
    }

    Song *curr = stack->top;
    int pos = 1;

    printf("\n========== PLAY HISTORY ==========\n");
    while (curr != NULL) {
        printf("%d) \"%s\" - %s (%d:%02d)\n",
               pos, curr->title, curr->artist,
               curr->duration / 60, curr->duration % 60);
        curr = curr->next;
        pos++;
    }
    printf("==================================\n\n");
}

void clearStack(Stack *stack) {
    Song *curr = stack->top;
    while (curr != NULL) {
        Song *temp = curr;
        curr = curr->next;
        free(temp);
    }

    stack->top = NULL;
}
