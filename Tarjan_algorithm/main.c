#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#define MAX_VERTEXES 1000

/*Topological sorting of a graph, Tarjan's algorithm*/

//table of color: 0=white(not checked), 1=grey(was, but once), 2=black(was more than once);
typedef struct Vector {
    int* data;
    int size;
    int capacity;
}
Vector;

void initializeVector(Vector* vector, int countOfVertex) {
    for (int i = 0; i < countOfVertex; i++) {
        vector[i].capacity = 10;
        vector[i].size = 0;
        vector[i].data = (int*)calloc(10, sizeof(int));
    }

}

void pushInVector(Vector* vector, int vertex) {
    if (vector->capacity < vector->size + 1) {
        vector->capacity = vector->capacity + 10;
        vector->data = (int*)realloc(vector->data, sizeof(int) * vector->capacity);
    }
    vector->data[vector->size] = vertex;
    vector->size++;
}


void sort(Vector* vector, int index, int* color_table, int* output, int* sizeOutput) {
    color_table[index] = 1;

    for (int i = 0; i < vector[index].size; i++) {
        if (color_table[vector[index].data[i]] == 2)
            break;
        if (color_table[vector[index].data[i]] == 1) {
            printf("impossible to sort");
            exit(EXIT_SUCCESS);
        }

        if (color_table[vector[index].data[i]] == 0)
            sort(vector, vector[index].data[i], color_table, output, sizeOutput);


    }

    color_table[index] = 2;
    output[*sizeOutput] = index + 1;
    *sizeOutput = *sizeOutput + 1;

}

void sortGraph(Vector* vector, int countOfVertex, int starterVertex, int* output) {
    int* color_table;
    color_table = (int*)calloc(countOfVertex, sizeof(int));
    
    int sizeOutput = 0;
    sort(vector, starterVertex, color_table, output, &sizeOutput);

    for (int i = 0; i < countOfVertex; i++) {
        if (color_table[i] == 0 && i != starterVertex && color_table[i] != 2)
            sort(vector, i, color_table, output, &sizeOutput);
    }
   
    free(color_table);
}

void printGraph(int* output, int countOfVertex) {
    for (int i = countOfVertex - 1; i >= 0; i--)
        printf("%d ", output[i]);
}

void removeVector(Vector** vector, int countOfVertex) {
    for (int i = 0; i < countOfVertex; i++)
        free((*vector)[i].data);

    free(*vector);
}



int main(void) {
    int countOfVertex = 0;
    int countOfEdge = 0;
    
    if (scanf("%d", &countOfVertex) == EOF) { printf("bad number of lines");  return 0; }
    if (scanf("%d", &countOfEdge) == EOF) { printf("bad number of lines");  return 0; }

    if (countOfVertex < 0 || countOfVertex > MAX_VERTEXES) { printf("bad number of vertices"); return 0; }
    if (countOfEdge<0 || countOfEdge > countOfVertex * (countOfVertex - 1) / 2) { printf("bad number of edges"); return 0; }

    Vector* vector = (Vector*)calloc(countOfVertex, sizeof(Vector));
    initializeVector(vector, countOfVertex);

    int beginVertex = 0;
    int endVertex = 0;
    int starterVertex = 0;
    for (int i = 0; i < countOfEdge; i++) {
        if (scanf("%d", &beginVertex) == EOF || scanf("%d", &endVertex) == EOF) { printf("bad number of lines"); removeVector(&vector, countOfVertex);  return 0; }
        if (beginVertex < 1 || beginVertex > countOfVertex || endVertex < 1 || endVertex > countOfVertex) { printf("bad vertex"); removeVector(&vector, countOfVertex); return 0; }
        if (i == 0)
            starterVertex = beginVertex - 1;

        pushInVector(&vector[beginVertex - 1], endVertex - 1);
    }
    int* output;
    output = (int*)calloc(countOfVertex, sizeof(int));

    sortGraph(vector, countOfVertex, starterVertex, output);
    printGraph(output, countOfVertex);

    free(output);
    removeVector(&vector, countOfVertex);
    return 0;
}
