#include "Queue.h"

Queue* InitQueue() {
	Queue* q = (Queue*)malloc(sizeof(Queue));
	if (!q) return NULL;
	q->headIndex=0;
	q->size=0;
	q->load=0;
	q->maxLoad=INITIAL_QUEUE_SIZE;
	q->entries=(void**)malloc(sizeof(void*)*INITIAL_QUEUE_SIZE);
	if (!q->entries) {
		free(q);
		return NULL;
	}	
	return q;
}

void DeleteQueue(Queue* q) {
	if (q) {
		if (q->entries) free(q->entries);
		free(q);
	}
}

void QueuePush(Queue* q,void* p) {
	if (q->load==q->maxLoad) {
		void** oldEntries = q->entries;
		q->entries=(void**)malloc(sizeof(void*)*q->maxLoad*2);
		if (!q->entries) return;
		memcpy(q->entries,oldEntries+q->headIndex,q->size*sizeof(void*));
		q->maxLoad*=2;
		q->load=q->size;
		q->headIndex=0;
		free(oldEntries);
//		printf("doubled\n");
	}
	q->size++;
	q->entries[q->load++]=p;
//	printf("Push %c\n",*(char*)p);
}

void* QueuePop(Queue* q) {
	if (q->size==0) return NULL;
	q->size--;
//	printf("Pop %c\n",*(char*)q->entries[q->headIndex]);
	return q->entries[q->headIndex++];
}
