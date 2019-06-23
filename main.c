#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <limits.h>
#include <locale.h>
#include <time.h>

#include "StackOfStack.h"
#include "IntStack.h"

#define FILENAME "graph.txt"

int **readFromFile(char *filename, int *size);

int bruteForce(int vertex, int **graph, int size, int *way);

int branchAndBound(int vertex, int **graph, int size, int *way);

int getUnvisitedNeighbour(int vertex, int **graph, int size, IntStack *currentWay, IntStack *visitedNeighbours);

int getNearestUnvisitedNeighbour(int vertex, int **graph, int size, IntStack *currentWay, IntStack *visitedNeighbours);

int *wayCopy(IntStack *way, int *dest);

void wayPrint(int *way, int size);

int nearestNeighbourMethod(int vertex, int **graph, int size, int *way);

int main() {
    setlocale(LC_ALL, "");
    int graphSize;
    int **graph = readFromFile(FILENAME, &graphSize);
    int *way = malloc(sizeof(int) * (graphSize + 1));
    int length;

    clock_t begin, end;
    begin = clock();
    length = bruteForce(1, graph, graphSize, way);
    end = clock();
    wprintf(L"Brute force found a way: ");
    wayPrint(way, graphSize + 1);
    wprintf(L"\n Its length = %d", length);
    wprintf(L"\n Elapsed %f", (double) (end - begin) / CLOCKS_PER_SEC);

    begin = clock();
    length = branchAndBound(1, graph, graphSize, way);
    end = clock();
    wprintf(L"\nBranch and bound method found a way: ");
    wayPrint(way, graphSize + 1);
    wprintf(L"\n Its length = %d", length);
    wprintf(L"\n Elapsed %f", (double) (end - begin) / CLOCKS_PER_SEC);

    begin = clock();
    length = nearestNeighbourMethod(1, graph, graphSize, way);
    end = clock();
    wprintf(L"\nNearest neighbour method found a way: ");
    wayPrint(way, graphSize + 1);
    wprintf(L"\n Its length = %d", length);
    wprintf(L"\n Elapsed %f", (double) (end - begin) / CLOCKS_PER_SEC);
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

int bruteForce(int vertex, int **graph, int size, int *way) {
    IntStack *actualWay = calloc(1, sizeof(IntStack));
    intStackPush(actualWay, vertex);
    StackOfStack *memory = calloc(1, sizeof(StackOfStack));
    stackOfStackPush(memory, calloc(1, sizeof(IntStack)));
    int current = vertex;
    int bestLength = INT_MAX;
    int wayLength = 0;
    int depth = 1;
    while (actualWay->head != NULL) {
        if ((current = getUnvisitedNeighbour(intStackTop(actualWay), graph, size, actualWay,
                                             stackOfStackTop(memory))) != -1) {
            wayLength += graph[intStackTop(actualWay)][current];
            intStackPush(actualWay, current);
            intStackPush(stackOfStackTop(memory), current);
            stackOfStackPush(memory, calloc(1, sizeof(IntStack)));
            depth++;
        } else {
            if (depth == size && graph[intStackTop(actualWay)][vertex] != 0 && wayLength < bestLength) {
                wayCopy(actualWay, way);
                bestLength = wayLength + graph[intStackTop(actualWay)][vertex];
            }
            wayLength -= graph[intStackPop(actualWay)][intStackTop(actualWay)];
            stackOfStackPop(memory);
            depth--;
        }
    }
    way[size] = vertex;
    return bestLength;
}

int branchAndBound(int vertex, int **graph, int size, int *way) {
    IntStack *actualWay = calloc(1, sizeof(IntStack));
    intStackPush(actualWay, vertex);
    StackOfStack *memory = calloc(1, sizeof(StackOfStack));
    stackOfStackPush(memory, calloc(1, sizeof(IntStack)));
    int current = vertex;
    int bestLength = INT_MAX;
    int wayLength = 0;
    int depth = 1;
    while (actualWay->head != NULL) {
        if ((current = getUnvisitedNeighbour(intStackTop(actualWay), graph, size, actualWay,
                                             stackOfStackTop(memory))) != -1 && wayLength < bestLength) {
            wayLength += graph[intStackTop(actualWay)][current];
            intStackPush(actualWay, current);
            intStackPush(stackOfStackTop(memory), current);
            stackOfStackPush(memory, calloc(1, sizeof(IntStack)));
            depth++;
        } else {
            if (depth == size && graph[intStackTop(actualWay)][vertex] != 0 && wayLength < bestLength) {
                wayCopy(actualWay, way);
                bestLength = wayLength + graph[intStackTop(actualWay)][vertex];
            }
            wayLength -= graph[intStackPop(actualWay)][intStackTop(actualWay)];
            stackOfStackPop(memory);
            depth--;
        }
    }
    way[size] = vertex;
    return bestLength;
}

int *wayCopy(IntStack *way, int *dest) {
    IntStackNode *current = way->head;
    for (int i = 0; current != NULL; current = current->next, i++) {
        dest[i] = current->data;
    }
    return dest;
}

void wayPrint(int *way, int size) {
    for (int i = 0; i < size; i++) {
        wprintf(L"%d ", way[i]);
    }
}

int getUnvisitedNeighbour(int vertex, int **graph, int size, IntStack *currentWay, IntStack *visitedNeighbours) {
    for (int i = 0; i < size; i++) {
        if (graph[vertex][i] != 0 && !isInIntStack(currentWay, i) && !isInIntStack(visitedNeighbours, i)) {
            return i;
        }
    }
    return -1;
}

int getNearestUnvisitedNeighbour(int vertex, int **graph, int size, IntStack *currentWay, IntStack *visitedNeighbours) {
    int longest = INT_MAX;
    int neighbour = -1;
    for (int i = 0; i < size; i++) {
        if (graph[vertex][i] != 0 && !isInIntStack(currentWay, i) && !isInIntStack(visitedNeighbours, i) &&
            graph[vertex][i] < longest) {
            longest = graph[vertex][i];
            neighbour = i;
        }
    }
    return neighbour;
}

int nearestNeighbourMethod(int vertex, int **graph, int size, int *way) {
    IntStack *actualWay = calloc(1, sizeof(IntStack));
    intStackPush(actualWay, vertex);
    StackOfStack *memory = calloc(1, sizeof(StackOfStack));
    stackOfStackPush(memory, calloc(1, sizeof(IntStack)));
    int current = vertex;
    int wayLength = 0;
    int depth = 1;
    while (actualWay->head != NULL) {
        if ((current = getNearestUnvisitedNeighbour(intStackTop(actualWay), graph, size, actualWay,
                                                    stackOfStackTop(memory))) != -1) {
            wayLength += graph[intStackTop(actualWay)][current];
            intStackPush(actualWay, current);
            intStackPush(stackOfStackTop(memory), current);
            stackOfStackPush(memory, calloc(1, sizeof(IntStack)));
            depth++;
        } else {
            if (depth == size && graph[intStackTop(actualWay)][vertex] != 0) {
                wayCopy(actualWay, way);
                way[size] = vertex;
                return wayLength + graph[intStackTop(actualWay)][vertex];
            }
            wayLength -= graph[intStackPop(actualWay)][intStackTop(actualWay)];
            stackOfStackPop(memory);
            depth--;
        }
    }
    return -1;
}