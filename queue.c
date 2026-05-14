/*
 * queue.c - Queue data structure for upcoming songs
 */

#include <stdio.h>
#include <stdlib.h>

#include "include/queue.h"

void initQueue(Queue *queue) {
    queue->front = NULL;
    queue->rear = NULL;
}

int isQueueEmpty(const Queue *queue) {
    return queue->front == NULL;
}

void enqueueSong(Queue *queue, const Song *song) {
    if (song == NULL) {
        printf("[queue] Cannot enqueue an empty song.\n");
        return;
    }

    Song *newSong = createSong(song->id, song->title, song->artist, song->duration);
    if (newSong == NULL) return;

    if (isQueueEmpty(queue)) {
        queue->front = newSong;
        queue->rear = newSong;
    } else {
        queue->rear->next = newSong;
        queue->rear = newSong;
    }

    printf("[queue] Added \"%s\" to play queue.\n", song->title);
}

Song *dequeueSong(Queue *queue) {
    if (isQueueEmpty(queue)) {
        printf("[queue] Play queue is empty.\n");
        return NULL;
    }

    Song *song = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    song->next = NULL;

    return song;
}

Song *peekQueue(const Queue *queue) {
    return queue->front;
}

void showQueue(const Queue *queue) {
    if (isQueueEmpty(queue)) {
        printf("  (Queue is empty)\n");
        return;
    }

    Song *curr = queue->front;
    int pos = 1;

    printf("\n========== PLAY QUEUE ==========\n");
    while (curr != NULL) {
        printf("%d) \"%s\" - %s (%d:%02d)\n",
               pos, curr->title, curr->artist,
               curr->duration / 60, curr->duration % 60);
        curr = curr->next;
        pos++;
    }
    printf("================================\n\n");
}

void clearQueue(Queue *queue) {
    Song *curr = queue->front;
    while (curr != NULL) {
        Song *temp = curr;
        curr = curr->next;
        free(temp);
    }

    queue->front = NULL;
    queue->rear = NULL;
}
