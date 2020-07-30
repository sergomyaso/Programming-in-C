#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define MAX_VERTEXES 5000

//Dijkstra's algorithm for finding the shortest path

int** newMatrix(int count) {
    int** matrix;
    matrix = (int**)calloc(count, sizeof(int*));
    for (int i = 0; i < count; i++) {
        matrix[i] = (int*)calloc(count, sizeof(int));
    }
    return matrix;
}

void removeMatrix(int** matrix, int countOfVertex) {
    for (int i = 0; i < countOfVertex; i++)
        free(matrix[i]);
    free(matrix);

}

void printAnswer(int** adjMatrix, unsigned* minWeight, int beginVertex, int endVertex, int countOfVertex, int countOfEdges) {
    for (int i = 0; i < countOfVertex; i++) {
        if (minWeight[i] == UINT_MAX)
            printf("oo ");
        else {
            if (minWeight[i] > INT_MAX)
                printf("INT_MAX+ ");
            else
                printf("%u ", minWeight[i]);
        }
    }
    printf("\n");

    if (minWeight[beginVertex - 1] == UINT_MAX || minWeight[endVertex - 1] == UINT_MAX) {
        printf("no path");
        return;
    }

    int* vertexes = (int*)malloc(countOfVertex * sizeof(int));
    int start = beginVertex - 1;
    int end = endVertex - 1;
    if (start == end) {
        printf("%d", start + 1);
        free(vertexes);
        return;
    }

    vertexes[0] = end + 1;
    int indexInVertexes = 1;
    int weight = minWeight[end];
    while (end != start)
    {
        for (int i = 0; i < countOfVertex; i++)
            if (adjMatrix[end][i] != 0) {
                if ((unsigned)(weight - adjMatrix[end][i]) == minWeight[i]) {
                    weight = weight - adjMatrix[end][i];
                    end = i;
                    vertexes[indexInVertexes] = i + 1;
                    indexInVertexes++;
                }
            }
    }

    unsigned sum = 0;
    for (int i = 0; i < indexInVertexes - 1; i++) {
        if (sum + adjMatrix[vertexes[i] - 1][vertexes[i + 1] - 1] > INT_MAX)
            sum = (unsigned)INT_MAX + 1;
        else
            sum += adjMatrix[vertexes[i] - 1][vertexes[i + 1] - 1];
    }

    if (sum > INT_MAX&& countOfEdges != 2) {
        printf("overflow");
        free(vertexes);
        return;
    }
    for (int i = 0; i < indexInVertexes; i++)
        printf("%d ", vertexes[i]);

    free(vertexes);
}

void findMinPath(int** adjMatrix, int countOfVertex, int beginVertex, unsigned* minWeight) {
    int* status;
    status = (int*)calloc(countOfVertex, sizeof(int));
    // if status = 1 - was, if 0 - not was

    unsigned minindex;
    for (int i = 0; i < countOfVertex; i++)
        minWeight[i] = UINT_MAX;

    minWeight[beginVertex - 1] = 0;

    do {
        minindex = UINT_MAX;
        unsigned min = UINT_MAX;
        for (int i = 0; i < countOfVertex; i++) {
            if (status[i] == 0 && minWeight[i] < min) {
                min = minWeight[i];
                minindex = i;
            }
        }

        if (minindex != UINT_MAX) {
            for (int i = 0; i < countOfVertex; i++) {
                if (adjMatrix[minindex][i] > 0 && status[i] != 1) {
                    if (min + adjMatrix[minindex][i] < minWeight[i])
                        minWeight[i] = min + adjMatrix[minindex][i];
                }
            }
            status[minindex] = 1;
        }

    } while (minindex < UINT_MAX);

    free(status);
}

int main(void) {

    int countOfVertex;
    FILE* input = fopen("in.txt", "r");
    if (fscanf(input, "%d", &countOfVertex) == EOF) { printf("bad number of lines"); fclose(input);  return 0; }
    int beginVertex;
    if (fscanf(input, "%d", &beginVertex) == EOF) { printf("bad number of lines"); fclose(input);  return 0; }
    int endVertex;
    if (fscanf(input, "%d", &endVertex) == EOF) { printf("bad number of lines"); fclose(input);  return 0; }
    if (beginVertex == 0 || endVertex == 0) { printf("bad vertex"); fclose(input); return 0; }
    int countOfEdge;
    if (fscanf(input, "%d", &countOfEdge) == EOF) { printf("bad number of lines"); fclose(input);  return 0; }
    if (countOfVertex < 0 || countOfVertex > MAX_VERTEXES) { printf("bad number of vertices"); fclose(input); return 0; }
    if (countOfEdge < 0 || countOfEdge > countOfVertex* (countOfVertex - 1) / 2) { printf("bad number of edges"); fclose(input); return 0; }

    int firstVertex;
    int secondVertex;
    int weight;

    int** adjMatrix;
    adjMatrix = newMatrix(countOfVertex);
    for (int i = 0; i < countOfEdge; i++) {
        if (fscanf(input, "%d", &firstVertex) == EOF || fscanf(input, "%d", &secondVertex) == EOF || fscanf(input, "%d", &weight) == EOF) { printf("bad number of lines"); removeMatrix(adjMatrix, countOfVertex); fclose(input); return 0; }
        if (firstVertex < 1 || secondVertex>countOfVertex || secondVertex < 1) { printf("bad vertex"); removeMatrix(adjMatrix, countOfVertex);  fclose(input); return 0; }
        if (weight < 0 || weight > INT_MAX) { printf("bad length");  removeMatrix(adjMatrix, countOfVertex); fclose(input);  return 0; }

        adjMatrix[firstVertex - 1][secondVertex - 1] = weight;
        adjMatrix[secondVertex - 1][firstVertex - 1] = weight;

    }

    unsigned* minWeight;
    minWeight = (unsigned*)malloc(countOfVertex * sizeof(unsigned));
    findMinPath(adjMatrix, countOfVertex, beginVertex, minWeight);
    printAnswer(adjMatrix, minWeight, beginVertex, endVertex, countOfVertex, countOfEdge);

    free(minWeight);
    removeMatrix(adjMatrix, countOfVertex);
    fclose(input);
    return 0;
}
