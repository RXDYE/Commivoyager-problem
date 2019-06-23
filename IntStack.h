#ifndef HALF_YEAR_WORK_INTSTACK_H
#define HALF_YEAR_WORK_INTSTACK_H

#include <stdlib.h>

typedef struct intStackNode {
    int data;
    struct intStackNode *next;
} IntStackNode;

typedef struct intStack {
    IntStackNode *head;
    IntStackNode *tail;
} IntStack;

void intStackPush(IntStack *intStack, int data) {
    if (intStack->head == NULL) {
        intStack->head = intStack->tail = malloc(sizeof(IntStackNode));
        intStack->head->data = data;
    } else {
        intStack->tail->next = malloc(sizeof(IntStackNode));
        intStack->tail = intStack->tail->next;
        intStack->tail->data = data;
    }
    intStack->tail->next = NULL;
}

int intStackTop(IntStack *intStack) {
    if (intStack->head == NULL) {
        return 0;
    } else return intStack->tail->data;
}

int intStackPop(IntStack *intStack) {
    if (intStack->head == NULL) {
        return 0;
    }
    int ret;
    if (intStack->head->next == NULL) {
        ret = intStack->head->data;
        free(intStack->head);
        intStack->head = intStack->tail == NULL;
    } else {
        IntStackNode *current = intStack->head;
        while (current->next != intStack->tail) {
            current = current->next;
        }
        ret = intStack->tail->data;
        free(intStack->tail);
        intStack->tail = current;
        intStack->tail->next = NULL;
    }
    return ret;
}

int isInIntStack(IntStack *intStack, int data) {
    if (intStack == NULL) {
        return 0;
    }
    if (intStack->head == NULL) {
        return 0;
    }
    IntStackNode *current = intStack->head;
    while (current != NULL) {
        if (current->data == data) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void intStackFree(IntStack *intStack) {
    if (intStack == NULL) {
        return;
    }
    IntStackNode *temp;
    IntStackNode *current;
    current = intStack->head;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
    free(intStack);
}

#endif //HALF_YEAR_WORK_INTSTACK_H
