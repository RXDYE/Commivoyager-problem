//
// Created by rxdye on 6/17/19.
//

#ifndef HALF_YEAR_WORK_STACK_H
#define HALF_YEAR_WORK_STACK_H

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node *next;
} Node;

Node *stackPush(Node *head, int data);

int stackPop(Node *head);

int stackTop(Node *head);

void stackFree(Node *head);

Node *stackPush(Node *head, int data) {
    if (head == NULL) {
        head = malloc(sizeof(Node));
        head->data = data;
        head->next = NULL;
    } else {
        Node *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = malloc(sizeof(Node));
        current->next->data = data;
        current->next->next = NULL;
    }
}

int stackPop(Node *head) {
    Node *current = head;
    while (current->next->next != NULL) {
        current = current->next;
    }
    int ret = current->next->data;
    free(current->next);
    current->next = NULL;
    return ret;
}

int stackTop(Node *head) {
    Node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    return current->data;
}

void stackFree(Node *head) {
    Node *current = head;
    Node *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

#endif //HALF_YEAR_WORK_STACK_H
