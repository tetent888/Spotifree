#ifndef QUEUE_H
#define QUEUE_H

#include "song.h"

typedef struct Queue {
    Song *front;
    Song *rear;
} Queue;

void initQueue(Queue *queue);
int isQueueEmpty(const Queue *queue);
void enqueueSong(Queue *queue, const Song *song);
Song *dequeueSong(Queue *queue);
Song *peekQueue(const Queue *queue);
void showQueue(const Queue *queue);
void clearQueue(Queue *queue);

#endif
