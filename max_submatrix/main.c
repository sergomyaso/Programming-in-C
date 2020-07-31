#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

// Find max submatrix, method dinamic programming 

int* NewArray(int count) {
    int* newArray;
    newArray = (int*)calloc(count, sizeof(int));
    assert(newArray);
    return newArray;
}

int** NewMatrix(int countCol, int countRow) {
    int** matrix;
    matrix = (int**)calloc(countRow, sizeof(int*));
    for (int i = 0; i < countRow; i++) {
        matrix[i] = NewArray(countCol);
    }
    return matrix;
}

void InitMatrix(int** matrix, int countCol, int countRow, FILE* input) {
    for (int i = 0; i < countRow; i++) {
        for (int j = 0; j < countCol; j++) {
            assert(fscanf(input, "%d", &matrix[i][j]));
        }
    }
}

void RemoveMatrix(int**matrix, int countRow) {
    for (int i = 0; i < countRow; i++) {
            free(matrix[i]);        
    }
    free(matrix);
  
}

int FindMaximumSubarray(int* intArray, int count) {

    int res = intArray[0];
    int sum = 0;
    for (int i = 0; i < count; i++) {
        sum += intArray[i];
        res = max(res, sum);
        sum = max(sum, 0);
    }
    return res;
}

int FindMaximumSubmatrix(int** matrix, int countRow, int countCol) {
    int currSum = INT_MIN;
    int sum = INT_MIN;
    int* subArray;
    subArray = NewArray(countCol);
    for (int currLeft = 0; currLeft < countRow; currLeft++) {
        memset(subArray, 0, sizeof(int) * countCol);

        for (int currRight = currLeft; currRight < countRow; currRight++) {

            for (int i = 0; i < countCol; i++) 
                subArray[i] += matrix[currRight][i];
         
            currSum = FindMaximumSubarray(subArray, countCol);
            sum=max(sum,currSum);
   
        }    

    }

    free(subArray);
    return sum;
}

int main(void) {
    FILE* input = fopen("in.txt", "r");
    int countRow = 0;
    assert(fscanf(input,"%d", &countRow));
    int countCol = 0;
    assert(fscanf(input, "%d", &countCol));
    int** matrix;
    matrix = NewMatrix(countCol,countRow);
    InitMatrix(matrix, countCol, countRow, input);
    printf("%d", FindMaximumSubmatrix(matrix, countRow, countCol));
    RemoveMatrix(matrix,countRow);
    fclose(input);

    return 0;
}
