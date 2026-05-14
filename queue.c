/*
 * queue.c - Play queue for next songs
 */

#include <stdio.h>
#include <stdlib.h>

#include "include/queue.h"

void queueInit(Queue *queue) {
    queue->front = NULL;
    queue->rear = NULL;
}

int queueIsEmpty(Queue *queue) {
    return queue->front == NULL;
}

void queueEnqueue(Queue *queue, const Song *song) {
    Song *newSong = createSong(song);

    if (newSong == NULL) {
        return;
    }

    if (queueIsEmpty(queue)) {
        queue->front = newSong;
        queue->rear = newSong;
    } else {
        queue->rear->next = newSong;
        queue->rear = newSong;
    }
}

Song *queueDequeue(Queue *queue) {
    Song *song;

    if (queueIsEmpty(queue)) {
        return NULL;
    }

    song = queue->front;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    song->next = NULL;
    return song;
}

Song *queuePeek(Queue *queue) {
    return queue->front;
}

void queueShow(Queue *queue) {
    Song *curr = queue->front;
    int number = 1;

    if (queueIsEmpty(queue)) {
        printf("(Queue is empty)\n");
        return;
    }

    printf("\n========== PLAY QUEUE ==========\n");
    while (curr != NULL) {
        printf("%d. [%s] %s - %s (%d:%02d)\n",
               number, curr->id, curr->title, curr->artist,
               curr->duration_seconds / 60, curr->duration_seconds % 60);
        curr = curr->next;
        number++;
    }
    printf("================================\n\n");
}

void queueClear(Queue *queue) {
    Song *curr = queue->front;

    while (curr != NULL) {
        Song *temp = curr;
        curr = curr->next;
        free(temp);
    }

    queue->front = NULL;
    queue->rear = NULL;
}
