#ifndef HALF_YEAR_WORK_STACKOFSTACK_H
#define HALF_YEAR_WORK_STACKOFSTACK_H

#include "IntStack.h"

typedef struct stackOfStackNode {
    IntStack *data;
    struct stackOfStackNode *next;
} StackOfStackNode;
typedef struct stackOfStack {
    StackOfStackNode *head;
    StackOfStackNode *tail;
} StackOfStack;

void stackOfStackPush(StackOfStack *stackOfStack, IntStack *data) {
    if (stackOfStack->head == NULL) {
        stackOfStack->head = (StackOfStackNode *) malloc(sizeof(StackOfStackNode));
        stackOfStack->head = stackOfStack->tail = malloc(sizeof(StackOfStackNode));
        stackOfStack->head->data = data;
    } else {
        stackOfStack->tail->next = malloc(sizeof(IntStackNode));
        stackOfStack->tail = stackOfStack->tail->next;
        stackOfStack->tail->data = data;
    }
    stackOfStack->tail->next = NULL;
}

IntStack *stackOfStackTop(StackOfStack *stackOfStack) {
    if (stackOfStack->head == NULL) {
        return NULL;
    } else return stackOfStack->tail->data;
}

void stackOfStackPop(StackOfStack *stackOfStack) {
    if (stackOfStack->head == NULL) {
        return;
    } else if (stackOfStack->head->next == NULL) {
        intStackFree(stackOfStack->head->data);
        free(stackOfStack->head);
        stackOfStack->head = stackOfStack->tail = NULL;
    } else {
        StackOfStackNode *current = stackOfStack->head;
        while (current->next != stackOfStack->tail) {
            current = current->next;
        }
        intStackFree(stackOfStack->tail->data);
        free(stackOfStack->tail);
        stackOfStack->tail = current;
        stackOfStack->tail->next = NULL;
    }
}

#endif //HALF_YEAR_WORK_STACKOFSTACK_H
