#ifndef QUEUE_H
#define QUEUE_H

#include "song.h"

typedef struct Queue {
    Song *front;
    Song *rear;
} Queue;

void queueInit(Queue *queue);
int queueIsEmpty(Queue *queue);
void queueEnqueue(Queue *queue, const Song *song);
Song *queueDequeue(Queue *queue);
Song *queuePeek(Queue *queue);
void queueShow(Queue *queue);
void queueClear(Queue *queue);

#endif
