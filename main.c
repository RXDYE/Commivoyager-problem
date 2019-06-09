#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>

#define FILENAME "graph.txt"

int **readFromFile(char *filename, int *size);

int **graphCopy(int **graph, int size);

int *branchAndBound(int **graph, int size, int *way);

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

int *branchAndBound(int **graph, int size, int *way) {
}