#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>

#include "stack.h"

#define FILENAME "graph.txt"

enum nodeState {
    raw, processed;
};

int **readFromFile(char *filename, int *size);

int **graphCopy(int **graph, int size);

//TODO
int *rough(int vertex, int **graph, int size, int *way);

int getUnvisitedNeighbour(int vertex, int **graph, int size, enum nodeState *states);

int isTraversed(enum nodeState *states, int size);

//TODO
int *nearestNeighbour(int vertex, int **graph, int size, int *way);

int main() {
    int graphSize;
    int **graph = readFromFile(FILENAME, &graphSize);
    return 0;
}

int **readFromFile(char *filename, int *size) {
    FILE *file = fopen(filename, "r");
    *size = 0;
    int cost;
    while (1) {
        fwscanf(file, L"%d", &cost);
        (*size)++;
        if (fgetwc(file) == L'\n') {
            break;
        }
    }

    int **graph = malloc(sizeof(int *) * (*size));
    fseek(file, 0, SEEK_SET);
    for (int i = 0; i < *size; i++) {
        graph[i] = malloc(sizeof(int) * (*size));
        for (int j = 0; j < *size; j++) {
            fwscanf(file, L"%d", &cost);
            graph[i][j] = cost;
        }
    }
    return graph;
}

int **graphCopy(int **graph, int size) {
    int **copy = malloc(sizeof(int) * (size));
    for (int i = 0; i < size; i++) {
        copy[i] = malloc(sizeof(int) * (size));
        for (int j = 0; j < size; j++) {
            copy[i][j] = graph[i][j];
        }
    }
    return copy;
}

int *rough(int vertex, int **graph, int size, int *way) {
    enum nodeState *states = calloc(size, sizeof(enum nodeState));
    Node *visited = NULL;
    stackPush(visited, vertex);
    int wayLength = 0;
    int visitedAmount = 0;
    int current;
    //TODO
    //maybe 3 states needed
    while (!isTraversed(states, size)) {
        current = stackTop(visited);
        stackPush(visited, getUnvisitedNeighbour(current, graph, size, states));
        states[stackTop(visited)] = processed;
        wayLength += graph[current][stackTop(visited)];
        visitedAmount++;
    }
}

int getUnvisitedNeighbour(int vertex, int **graph, int size, enum nodeState *states) {
    for (int i = 0; i < size; i++) {
        if (graph[vertex][i] != 0 && states[i] == raw) {
            return i;
        }
    }
    return -1;
}

int isTraversed(enum nodeState *states, int size) {
    for (int i = 0; i < size; i++) {
        if (states[i] == raw) {
            return 0;
        }
    }
    return 1;
}

//TODO
//int *nearestNeighbour(int vertex, int **graph, int size, int *way) {
//    nodeState *states = calloc(size, sizeof(enum nodeState));
//
//}