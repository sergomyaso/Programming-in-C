#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#define uchar unsigned char
#define SIZE_P 18 /*size of pattern*/
#define SIZE_B 10000 /*size of buffer*/
#define HASH_N 3 /*number for hash*/

/*Rabin-Karp algorithm for searching substrings in a string*/

int GetPower(int num, int power) {
    int res = 1;
    while (power) {
        if (power % 2) {
            res *= num;
            power--;
        }
        else {
            num *= num;
            power /= 2;
        }
    }
    return res;
}

int GetHash(uchar* str, int lengthString) {
    int hash = 0;
    for (int i = 0; i < lengthString; i++) {
        hash = hash + (str[i] % HASH_N) * GetPower(HASH_N, i);
    }
    return hash;
}

void RecalculateHashSubstr(int* currentHash, uchar beginSubstr, uchar endSubstr, int maxPower) {
    (*currentHash) -= ((int)beginSubstr % HASH_N);
    (*currentHash) = (*currentHash) / 3;
    (*currentHash) = (*currentHash) + (endSubstr % 3) * maxPower;
}


void PrintPositions(FILE* output, uchar* pattern, uchar* currentSubstr, int count, int indexFile) {
    for (int i = 0; i < count; i++) {
        if (pattern[i] == currentSubstr[i]) {
            fprintf(output, "%d ", indexFile + i);
        }
        else {
            fprintf(output, "%d ", indexFile + i);
            return;
        }

    }
}

void FindSubstr(FILE* input, FILE* output, uchar* pattern) {
    int lengthPattern = (int)strlen((char*)pattern);
    int maxPower = (int)GetPower(HASH_N, lengthPattern - 1);
    uchar buffer[SIZE_B + 1];
    int hashPattern = GetHash(pattern, lengthPattern);

    fprintf(output, "%d ", hashPattern);
    int indexFile = 1;
    uchar currentSubstr[SIZE_P + 1];
    if (fread(currentSubstr, sizeof(uchar), lengthPattern, input) == 0)
        return;

    int hashSubstr = GetHash(currentSubstr, lengthPattern);
    if (hashPattern == hashSubstr)
        PrintPositions(output, pattern, currentSubstr, lengthPattern, indexFile);

    while (1) {
        int sizeBuffer = (int)fread(buffer, sizeof(uchar), SIZE_B, input);
        for (int i = 0; i < sizeBuffer; i++) {
            indexFile++;
            RecalculateHashSubstr(&hashSubstr, currentSubstr[0], buffer[i], maxPower);
            memmove(currentSubstr, currentSubstr + 1, sizeof(uchar) * (lengthPattern - 1));
            currentSubstr[lengthPattern - 1] = buffer[i];

            if (hashPattern == hashSubstr) {
                PrintPositions(output, pattern, currentSubstr, lengthPattern, indexFile);
            }
        }
        if (sizeBuffer < SIZE_B)
            break;
    }
}

int main() {
    uchar pattern[SIZE_P + 1];
    FILE* input = fopen("in.txt", "r");
    assert(input);
    FILE* output = fopen("out.txt", "w");
    assert(output);
    if (fgets((char*)pattern, SIZE_P, input) == NULL) { fclose(input); fclose(output); return 0; }
    pattern[strlen((char*)pattern) - 1] = '\0';
    FindSubstr(input, output, pattern);

    fclose(input);
    fclose(output);
    return 0;

}
