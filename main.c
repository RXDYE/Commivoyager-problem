#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>

#define FILENAME "graph.txt"

int **readFromFile(char *filename, int *size) {
    FILE *file = fopen(filename, "r");

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

int main() {
    int graphSize = 0;
    int **graph = readFromFile(FILENAME, &graphSize);
    for (int i = 0; i < graphSize; i++) {
        for (int j = 0; j < graphSize; j++) {
            wprintf(L"%d", graph[i][j]);
        }
        wprintf(L"\n");
    }
    return 0;
}