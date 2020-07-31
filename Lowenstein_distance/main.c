#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

// Find to Lowenstein distance, method of dinamic programming

int* NewArray(int count) {
    int* newArray;
    newArray = (int*)calloc(count, sizeof(int));
    assert(newArray);
    return newArray;
}

int** NewMatrix(int countCol, int countRow) {
    int** matrix;
    matrix = (int**)calloc(countRow, sizeof(int*));
    assert(matrix);
    for (int i = 0; i < countRow; i++) {
        matrix[i] = NewArray(countCol);
    }
    return matrix;
}

void RemoveMatrix(int**matrix, int countRow) {
    for (int i = 0; i < countRow; i++) {
            free(matrix[i]);        
    }
    free(matrix);
  
}

 void printActualEdits(int** T, char* str1, char* str2) {
    int i = (int)strlen(str1);
    int j = (int)strlen(str2);
    while (1) {
        if (i == 0 || j == 0) {
            break;
        }
        if (str1[i - 1] == str2[j - 1]) {
            i = i - 1;
            j = j - 1;
        }
        else if (T[i][j] == T[i - 1][j - 1] + 1) {
            printf("\nEdit %c in string2 to %c  in string1",  str2[j - 1] , str1[i - 1] );
            i = i - 1;
            j = j - 1;
        }
        else if (T[i][j] == T[i - 1][j] + 1) {
            printf("\nDelete in string1 %c", str1[i - 1]);
            i = i - 1;
        }
        else if (T[i][j] == T[i][j - 1] + 1) {
            printf("\nDelete in string2 %c", str2[j - 1]);
            j = j - 1;
        }
        

    }
}

int GetLowensteinDist(char* str1, char* str2) {
    int** matrix;
    int lengthFirstStr = (int)strlen(str1);
    int lengthSecondStr = (int)strlen(str2);
    matrix = NewMatrix(lengthSecondStr + 1, lengthFirstStr + 1);

    for (int i = 0; i < lengthFirstStr; i++) {
        matrix[0][i] = i;
    }
    for (int i = 0; i < lengthSecondStr; i++) {
        matrix[i][0] = i;
    }

    for (int i = 1; i <= lengthFirstStr ; i++) {
        for (int j = 1; j <= lengthSecondStr; j++) {
            if (str1[i-1] == str2[j-1]) {
                matrix[i][j] = matrix[i - 1][j - 1];
            }
            else {
                matrix[i][j] = 1 + min(min(matrix[i][j - 1], matrix[i - 1][j - 1]), matrix[i - 1][j]);
            }

        }

    }
    int result = 0;
    result = matrix[lengthFirstStr][lengthSecondStr];
    printActualEdits(matrix, str1, str2);
    RemoveMatrix(matrix,lengthFirstStr);
    return result;

}

int main(void) {
    char str1[256];
    char str2[256];
    assert(scanf("%s", str1));
    assert(scanf("%s", str2));
    printf("\n\n%d", GetLowensteinDist(str1, str2));

    return 0;
}
