#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define MAX_VERTEXES 5000
// Kruskal's algorithm for find spanning tree

typedef struct Edge {
    int beginVertex;
    int endVertex;
    int weight;
}
Edge;

void setEdge(Edge* edge, int beginVertex, int endVertex, int weight) {
    edge->beginVertex = beginVertex;
    edge->endVertex = endVertex;
    edge->weight = weight;

}

void swapEdge(Edge* first, Edge* second) {
    Edge swapVar;
    swapVar = *first;
    *first = *second;
    *second = swapVar;

}

void makeHeap(Edge* edges, int count, int i) {
    int largest = i;
    int left = (2 * i) + 1;
    int right = (2 * i) + 2;
    if (left < count && edges[largest].weight < edges[left].weight)
        largest = left;

    if (right < count && edges[largest].weight < edges[right].weight)
        largest = right;

    if (largest != i) {
        swapEdge(&edges[largest], &edges[i]);

        makeHeap(edges, count, largest);

    }
}

void sortEdges(Edge* edges, int count) {
    int i;
    int n = count;
    for (i = n / 2 - 1; i >= 0; i--)
        makeHeap(edges, n, i);

    for (i = 0; i < n; i++) {
        swapEdge(&edges[0], &edges[count - 1]);
        count--;
        makeHeap(edges, count, 0);
    }

}

void initDS(int* parent, int* rank, int countOfVertex) {
    for (int i = 0; i < countOfVertex; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

}

int find(int* parent, int vertex) {
    if (parent[vertex] == vertex)
        return vertex;

   return find(parent, parent[vertex]);

}

int uniteSet(int* parent, int* rank, int beginVertex, int endVertex) {
    beginVertex = find(parent, beginVertex);
    endVertex = find(parent, endVertex);
    if (beginVertex == endVertex) return 0;

    if (rank[beginVertex] < rank[endVertex]) {
        parent[beginVertex] = endVertex;
    }
    else {
        parent[endVertex] = beginVertex;
        if (rank[beginVertex] == rank[endVertex])
            rank[beginVertex]++;
    }
    return 1;
}

int findSpanningTree(Edge* edges, int countOfEdge, int countOfVertex, int* output) {
    int* parent;
    int* rank;
    parent = (int*)calloc(countOfVertex, sizeof(int));
    rank = (int*)calloc(countOfVertex, sizeof(int));
    int indexOutput = 0;
    initDS(parent, rank, countOfVertex);
    for (int i = 0; i < countOfEdge; i++) {
        if (uniteSet(parent, rank, edges[i].beginVertex - 1, edges[i].endVertex - 1) == 1) {
            output[indexOutput] = i;
            indexOutput++;
        }
        if (indexOutput + 1 == countOfVertex) break;

    }

    free(parent);
    free(rank);

    if (indexOutput + 1 != countOfVertex)
        return 0;

    return 1;
}

void printTree(Edge* edges, int* output, int countOfVertex) {
    for (int i = 0; i < countOfVertex - 1; i++) {
        printf("%d ", edges[output[i]].beginVertex);
        printf("%d \n", edges[output[i]].endVertex);
    }

}

int main(void) {
    int countOfVertex = 0;
    int countOfEdge = 0;

    if (scanf("%d", &countOfVertex) == EOF) { printf("bad number of lines");  return 0; }
    if (scanf("%d", &countOfEdge) == EOF) { printf("bad number of lines");  return 0; }
    if (countOfVertex < 0 || countOfVertex > MAX_VERTEXES) { printf("bad number of vertices"); return 0; }
    if (countOfEdge<0 || countOfEdge > countOfVertex * (countOfVertex - 1) / 2) { printf("bad number of edges"); return 0; }
    if (countOfVertex == 1 && countOfEdge == 0) return 0;
    if (countOfVertex == 0 || countOfEdge < countOfVertex - 1) { printf("no spanning tree"); return 0; }

    Edge* edges;
    edges = (Edge*)calloc(countOfEdge, sizeof(Edge));

    int beginVertex = 0;
    int endVertex = 0;
    int weightEdge;

    for (int i = 0; i < countOfEdge; i++) {
        if (scanf("%d", &beginVertex) == EOF || scanf("%d", &endVertex) == EOF || scanf("%d", &weightEdge) == EOF) { printf("bad number of lines"); free(edges);   return 0; }
        if (beginVertex < 1 || beginVertex > countOfVertex || endVertex < 1 || endVertex > countOfVertex) { printf("bad vertex"); free(edges);  return 0; }
        if (weightEdge < 0 || weightEdge > INT_MAX) { printf("bad length"); free(edges); return 0; }
        setEdge(&edges[i], beginVertex, endVertex, weightEdge);

    }
    int* output;
    output = (int*)calloc((size_t)(countOfVertex - 1), sizeof(int)); // Answer have count of vertex without 1 (N-1)
    sortEdges(edges, countOfEdge);

    if (findSpanningTree(edges, countOfEdge, countOfVertex, output))
        printTree(edges, output, countOfVertex);
    else
        printf("no spanning tree");

    free(output);
    free(edges);
    return 0;
}
