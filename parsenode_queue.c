#include <stdlib.h>

#include "parser.h"
#define MAX 500

ParseNode* queue[MAX];
int front = 0, rear = 0;
int size = 0;

int isFull() {
    if ((rear + 1) % MAX == front) return 1;
    return 0;
}

int isEmpty() {
    if (front == rear) return 1;
    return 0;
}

ParseNode* dequeue() {
    if (isEmpty()) return NULL;
    front = (front + 1) % MAX;
    size--;
    return queue[front];
}

int enqueue(ParseNode* pn) {
    if (isFull()) return 0;

    rear = (rear + 1) % MAX;
    queue[rear] = pn;
    size++;
    return 1;
}

int get_size() {
    return size;
}
