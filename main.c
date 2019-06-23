#include <stdio.h>
#include <string.h>
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

int isOriented(int **graph, int size);

void graphDelete(int **graph, int size);

int main() {
    int graphSize;
    int length;
    int mode;
    char filename[40];
    clock_t begin, end;

    printf("Read from default file? 1 - yes, other input - to enter your filename\n");
    scanf("%d", &mode);
    if (mode != 1) {
        printf("Please, enter the name of the file\n");
        scanf("%s", filename);
    } else {
        strcpy(filename, FILENAME);
    }
    int **graph = readFromFile(filename, &graphSize);
    int *way = malloc(sizeof(int) * (graphSize + 1));

    
    if (graph == NULL) {
        printf("Error while reading a file. File was not found or there is incorrect data");
        return 0;
    }
    if (isOriented(graph, graphSize)) {
        printf("This graph is oriented");
        return 0;
    }


    begin = clock();
    length = bruteForce(1, graph, graphSize, way);
    end = clock();
    if (length == -1) {
        printf("This graph has no hamilton cycle. Commivoyager problem can't be solved\n");
        return 0;
    }
    printf("Brute force found a way: ");
    wayPrint(way, graphSize + 1);
    printf("\n Its length = %d\n", length);
    printf(" Elapsed %f\n", (double) (end - begin) / CLOCKS_PER_SEC);


    begin = clock();
    length = branchAndBound(1, graph, graphSize, way);
    end = clock();
    printf("Branch and bound method found a way: ");
    wayPrint(way, graphSize + 1);
    printf("\n Its length = %d\n", length);
    printf(" Elapsed %f\n", (double) (end - begin) / CLOCKS_PER_SEC);


    begin = clock();
    length = nearestNeighbourMethod(1, graph, graphSize, way);
    end = clock();
    printf("Nearest neighbour method found a way: ");
    wayPrint(way, graphSize + 1);
    printf("\n Its length = %d\n", length);
    printf(" Elapsed %f", (double) (end - begin) / CLOCKS_PER_SEC);


    free(way);
    graphDelete(graph, graphSize);
    return 0;
}

int **readFromFile(char *filename, int *size) {
    FILE *file = fopen(filename, "r");
    *size = 0;
    int temp;
    while (1) {
        fscanf(file, "%d", &temp);
        (*size)++;
        if (fgetc(file) == L'\n') {
            break;
        }
    }

    int **graph = malloc(sizeof(int *) * (*size));
    fseek(file, 0, SEEK_SET);
    for (int i = 0; i < *size; i++) {
        graph[i] = malloc(sizeof(int) * (*size));
        for (int j = 0; j < *size; j++) {
            if (feof(file)) {
                while (i >= 0) {
                    free(graph[i]);
                    i--;
                }
                free(graph);
                return NULL;
            }
            fscanf(file, "%d", &graph[i][j]);
        }
    }
    return graph;
}

int bruteForce(int vertex, int **graph, int size, int *way) {
    int current = vertex;
    IntStack *actualWay = calloc(1, sizeof(IntStack));
    intStackPush(actualWay, vertex);
    StackOfStack *memory = calloc(1, sizeof(StackOfStack));
    stackOfStackPush(memory, calloc(1, sizeof(IntStack)));
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
            if (depth == size && graph[intStackTop(actualWay)][vertex] != 0 &&
                wayLength + graph[intStackTop(actualWay)][vertex] < bestLength) {
                wayCopy(actualWay, way);
                bestLength = wayLength + graph[intStackTop(actualWay)][vertex];
            }
            wayLength -= graph[intStackPop(actualWay)][intStackTop(actualWay)];
            stackOfStackPop(memory);
            depth--;
        }
    }
    way[size] = vertex;
    intStackFree(actualWay);
    stackOfStackFree(memory);
    if (bestLength == INT_MAX) {
        return -1;
    }
    return bestLength;
}

int branchAndBound(int vertex, int **graph, int size, int *way) {
    int current = vertex;
    IntStack *actualWay = calloc(1, sizeof(IntStack));
    intStackPush(actualWay, vertex);
    StackOfStack *memory = calloc(1, sizeof(StackOfStack));
    stackOfStackPush(memory, calloc(1, sizeof(IntStack)));
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
            if (depth == size && graph[intStackTop(actualWay)][vertex] != 0 &&
                wayLength + graph[intStackTop(actualWay)][vertex] < bestLength) {
                wayCopy(actualWay, way);
                bestLength = wayLength + graph[intStackTop(actualWay)][vertex];
            }
            wayLength -= graph[intStackPop(actualWay)][intStackTop(actualWay)];
            stackOfStackPop(memory);
            depth--;
        }
    }
    way[size] = vertex;
    intStackFree(actualWay);
    stackOfStackFree(memory);
    if (bestLength == INT_MAX) {
        return -1;
    }
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
        printf("%d ", way[i]);
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
    int current = vertex;
    IntStack *actualWay = calloc(1, sizeof(IntStack));
    intStackPush(actualWay, vertex);
    StackOfStack *memory = calloc(1, sizeof(StackOfStack));
    stackOfStackPush(memory, calloc(1, sizeof(IntStack)));
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
                current = intStackTop(actualWay);
                intStackFree(actualWay);
                stackOfStackFree(memory);
                return wayLength + graph[current][vertex];
            }
            wayLength -= graph[intStackPop(actualWay)][intStackTop(actualWay)];
            stackOfStackPop(memory);
            depth--;
        }
    }
    intStackFree(actualWay);
    stackOfStackFree(memory);
    return -1;
}

int isOriented(int **graph, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (graph[i][j] != graph[j][i]) {
                return 1;
            }
        }
    }
    return 0;
}

void graphDelete(int **graph, int size) {
    for (int i = 0; i < size; i++) {
        free(graph[i]);
    }
    free(graph);
}