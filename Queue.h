#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_QUEUE_SIZE 8

typedef struct {
	int size;
	int headIndex;
	int load;
	int maxLoad;
	void** entries;
} Queue;

Queue* InitQueue();
void* QueuePop(Queue* q);
void QueuePush(Queue* q,void* p);
void DeleteQueue(Queue* q);
