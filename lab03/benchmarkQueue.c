
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"


int main(void) {
	Queue q = QueueNew();

	// TODO
	// Write a benchmark test to demonstrate the difference between
	// ArrayQueue and CircularArrayQueue

	// enqueue 1 to 100000
	for (int i = 1; i <= 100000; i++) {
		QueueEnqueue(q, i);
	}

	// dequeue 1 to 5000
	for (int j = 1; j <= 5000; j++) {
		QueueDequeue(q);
	}

	QueueFree(q);
}

