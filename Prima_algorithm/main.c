#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define MAX_VERTEXES 5000
//Prima's algorithm for find spanning tree

int* newMatrix(int rows, int columns) {
    int* matrix;
    matrix = calloc(rows * columns, sizeof(int));
    return matrix;
}

void removeMatrix(int* matrix) {  
    free(matrix);
}

int getMinIndex(unsigned* weightArr, char* status, int countOfVertex) {
    unsigned min = (unsigned)INT_MAX + 1;
    int minIndex = -1; // First vertex don't have parent, is begin
    for (int i = 0; i < countOfVertex; i++) {
        if (status[i] == 0 && min > weightArr[i]) {
            min = weightArr[i];
            minIndex = i;
        }
    }
    return minIndex;
}

char findSpanningTree(int* graph, int countOfVertex, int* parent) {
    char* status;
    unsigned* weightArr;

    status = (char*)calloc(countOfVertex, sizeof(char));
    weightArr = (unsigned*)calloc(countOfVertex, sizeof(unsigned));

    for (int i = 0; i < countOfVertex; i++) {
        weightArr[i] = (unsigned)INT_MAX + 1;
        status[i] = 0;
    }

    weightArr[0] = 0;
    parent[0] = -1;

    for (int i = 0; i < countOfVertex - 1; i++) {
        int minIndex = getMinIndex(weightArr, status, countOfVertex);
        status[minIndex] = 1;

        for (int j = 0; j < countOfVertex; j++)
            if (graph[minIndex * countOfVertex + j] && status[j] == 0 && (unsigned)graph[minIndex * countOfVertex + j] < weightArr[j]) {
                parent[j] = minIndex;
                weightArr[j] = graph[minIndex * countOfVertex + j];
            }

    }

    for (int i = 0; i < countOfVertex; i++)
        if (weightArr[i] == (unsigned)INT_MAX + 1) {
            printf("no spanning tree");
            free(weightArr);
            free(status);
            return 0;
        }

    free(weightArr);
    free(status);
    return 1;

}

void printTree(int* parent, int countOfVertex) {
    for (int i = 1; i < countOfVertex; i++) { // loop begin from 1, because first vertex don't have parent
        printf("\n%d ", parent[i] + 1);
        printf("%d", i + 1);
    }

}

int main(void) {
    int countOfVertex = 0;
    int countOfEdge = 0;
    FILE* input = fopen("in.txt", "r");
    if (fscanf(input, "%d", &countOfVertex) == EOF) { printf("bad number of lines"); fclose(input);  return 0; }
    if (fscanf(input, "%d", &countOfEdge) == EOF) { printf("bad number of lines"); fclose(input);  return 0; }

    if (countOfVertex < 0 || countOfVertex > MAX_VERTEXES) { printf("bad number of vertices"); fclose(input); return 0; }
    if (countOfEdge < 0 || countOfEdge > countOfVertex * (countOfVertex - 1) / 2) { printf("bad number of edges"); fclose(input); return 0; }
    if (countOfVertex == 1 && countOfEdge == 0) { fclose(input); return 0; }
    if (countOfVertex == 0 || countOfEdge < countOfVertex - 1) { printf("no spanning tree"); fclose(input); return 0; }

    int beginVertex = 0;
    int endVertex = 0;
    int weightEdge = 0;

    int* adjMatrix;
    adjMatrix = newMatrix(countOfVertex, countOfVertex);

    for (int i = 0; i < countOfEdge; i++) {
        if (3 != fscanf(input, "%d%d%d", &beginVertex, &endVertex, &weightEdge)) { printf("bad number of lines"); removeMatrix(adjMatrix); fclose(input); return 0; }
        if (beginVertex < 1 || beginVertex > countOfVertex || endVertex<1 || endVertex > countOfVertex) { printf("bad vertex"); removeMatrix(adjMatrix); fclose(input);   return 0; }
        if (weightEdge < 0 || weightEdge > INT_MAX) { printf("bad length"); removeMatrix(adjMatrix); fclose(input);  return 0; }
        adjMatrix[(beginVertex - 1) * countOfVertex + (endVertex - 1)] = weightEdge;
        adjMatrix[(endVertex - 1) * countOfVertex + (beginVertex - 1)] = weightEdge;

    }

    int* parent;
    parent = (int*)calloc(countOfVertex, sizeof(int));

    if (!findSpanningTree(adjMatrix, countOfVertex, parent))
        printf("no spanning tree");
    else
        printTree(parent, countOfVertex);

    removeMatrix(adjMatrix);
    free(parent);
    fclose(input);
    return 0;
}
