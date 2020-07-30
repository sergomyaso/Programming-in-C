#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#define uchar unsigned char
#define LETTER_COUNT 256
/*Archiver based on the Huffman compression algorithm*/
typedef struct HuffTree {
    uchar symb;
    unsigned int count;
    struct HuffTree* leftChild;
    struct HuffTree* rightChild;
}
HuffTree;

typedef struct  Code {
    uchar* code;
    int size;
}
Code;

typedef struct BitContainer {
    uchar symb;
    int size;
}
BitContainer;

BitContainer* NewBitContainer() {
    BitContainer* container = (BitContainer*)calloc(1, sizeof(BitContainer));
    assert(container);
    container->symb = 0;
    container->size = 0;
    return container;
}

HuffTree* NewTree() {
    HuffTree* tree;
    tree = (HuffTree*)calloc(1, sizeof(HuffTree));
    assert(tree);

    return tree;
}

void InitTree(HuffTree* tree, uchar symb, int count, HuffTree* leftChild, HuffTree* rightChild) {
    tree->count = count;
    tree->symb = symb;
    tree->leftChild = leftChild;
    tree->rightChild = rightChild;
}

void RemoveTree(HuffTree* tree) {
    if (!tree)
        return;
    if (tree->leftChild)
        RemoveTree(tree->leftChild);
    if (tree->rightChild)
        RemoveTree(tree->rightChild);
    free(tree);
}

HuffTree** NewTreesArray() {
    HuffTree** trees;
    trees = (HuffTree**)calloc(LETTER_COUNT, sizeof(HuffTree*));
    assert(trees);

    return trees;
}

void RemoveTreesArray(HuffTree** trees) {
    RemoveTree(trees[0]);
    free(trees);
}

void SwapTrees(HuffTree** fisrtTree, HuffTree** secondTree) {
    HuffTree* swapVar;
    swapVar = *fisrtTree;
    *fisrtTree = *secondTree;
    *secondTree = swapVar;
}

void MakeHeap(HuffTree** trees, int count, int i) {
    int largest = i;
    int left = (2 * i) + 1;
    int right = (2 * i) + 2;
    if (left < count && trees[largest]->count < trees[left]->count)
        largest = left;
    if (right < count && trees[largest]->count < trees[right]->count)
        largest = right;
    if (largest != i) {
        SwapTrees(&trees[largest], &trees[i]);

        MakeHeap(trees, count, largest);
    }
}

void SortTrees(HuffTree** trees, int count) {
    int i;
    int n = count;
    for (i = n / 2 - 1; i >= 0; i--)
        MakeHeap(trees, n, i);

    for (i = 0; i < n; i++) {
        SwapTrees(&trees[0], &trees[count - 1]);
        count--;
        MakeHeap(trees, count, 0);
    }
}

int* NewIntArray(int countOfElements) {
    int* newArray;
    newArray = (int*)calloc(countOfElements, sizeof(int));
    assert(newArray);
    return newArray;
}

void FillIntArray(int* symbTable, FILE* input) {
    while (!feof(input)) {
        symbTable[(uchar)fgetc(input)]++;
    }
    symbTable[(uchar)EOF]--; // for don't count EOF
    fseek(input, 3, SEEK_SET);
}


void InitTreesArray(HuffTree** trees, int* symbTable, int* size) {
    for (int i = 0; i < LETTER_COUNT; i++) {
        if (symbTable[i] > 0) {
            trees[(*size)] = NewTree();
            InitTree(trees[(*size)], (uchar)i, symbTable[i], NULL, NULL);
            (*size)++;
        }

    }
    SortTrees(trees, (*size));

}

void UniteTrees(HuffTree** trees) {
    HuffTree* unit;
    unit = NewTree();
    unit->count = trees[0]->count + trees[1]->count;
    unit->leftChild = trees[1];
    unit->rightChild = trees[0];
    trees[0] = unit;
}

void BuildTree(HuffTree** trees, int size) {
    int index = 0;

    while (size > 1) {
        UniteTrees(trees);
        SwapTrees(&trees[index + 1], &trees[size - 1]);
        size--;
        SortTrees(trees, size);
    }

}

void MakeTree(HuffTree** trees, int* symbTable) {
    int size = 0;
    InitTreesArray(trees, symbTable, &size); //Every character is separate tree in trees array
    BuildTree(trees, size); //The main tree is accumulated in the first element of the tree array

}

Code* NewCodeElement(uchar* code, int size) {
    Code* element;
    element = (Code*)calloc(1, sizeof(Code));
    assert(element);
    element->code = code;
    element->size = size;
    return element;

}

Code** NewCodeArray(int count) {
    Code** CodeArray;
    CodeArray = (Code**)calloc(count, sizeof(Code*));
    return CodeArray;
}

void RemoveCodeArray(Code** codeTable, int count) {
    for (int i = 0; i < count; i++) {
        if (codeTable[i]) {
            free(codeTable[i]->code);
            free(codeTable[i]);
        }
    }
    free(codeTable);
}

void ResizeBuffer(Code* buffer) {
    buffer->size++;
    int isOverflow = buffer->size % CHAR_BIT;
    if (isOverflow == 1) {
        int bytes = buffer->size / CHAR_BIT + 1;
        buffer->code = (unsigned char*)realloc(buffer->code, (bytes + 5) * sizeof(uchar));
        buffer->code[bytes - 1] = 0;
    }

}

unsigned char* CopyCodeElement(Code* element) {
    int bytes = (element->size - 1) / CHAR_BIT + 1;
    unsigned char* newSymb = (unsigned char*)malloc((bytes + 5) * sizeof(uchar));
    assert(newSymb);
    for (int i = 0; i < bytes; i++) {
        newSymb[i] = element->code[i];
    }
    return newSymb;
}

void AddBitInCodeElement(Code* element, int bit) {
    int pos = (element->size - 1) / CHAR_BIT;
    element->code[pos] = (element->code[pos] << 1) + (unsigned char)bit;
}

void BuildCodeTable(Code** codeTable, HuffTree* codeTree, Code* buffer) {
    if (!codeTree) {
        return;
    }
    if (!codeTree->leftChild) {
        if (!buffer->code) {
            ResizeBuffer(buffer);
        }
        codeTable[codeTree->symb] = buffer;
    }
    else {
        ResizeBuffer(buffer);
        Code* newBuff = NewCodeElement(CopyCodeElement(buffer), buffer->size);
        AddBitInCodeElement(buffer, 0);
        AddBitInCodeElement(newBuff, 1);
        BuildCodeTable(codeTable, codeTree->leftChild, buffer);
        BuildCodeTable(codeTable, codeTree->rightChild, newBuff);
    }
}

void WriteBits(FILE* output, BitContainer* bitContainer, int countBits, uchar bit) {
    if (countBits == 0)
        return;

    int freeBits;
    freeBits = (int)CHAR_BIT - bitContainer->size;
    if (freeBits >= countBits) {
        uchar mask;
        bitContainer->symb = bitContainer->symb << countBits;
        mask = (1 << countBits) - 1;
        bitContainer->symb += (bit & mask);
        bitContainer->size += countBits;
        if (bitContainer->size == (int)CHAR_BIT) {
            fputc(bitContainer->symb, output);
            bitContainer->size = 0;
            bitContainer->symb = 0;
        }

    }
    else {
        WriteBits(output, bitContainer, freeBits, bit >> (countBits - freeBits));
        WriteBits(output, bitContainer, countBits - freeBits, bit);
    }

}

void SaveTree(FILE* output, HuffTree* tree, BitContainer* bitContatiner) {
    if (!tree)
        return;

    if (!tree->leftChild) {
        WriteBits(output, bitContatiner, 1, 0);
        WriteBits(output, bitContatiner, CHAR_BIT, tree->symb);
        return;
    }

    WriteBits(output, bitContatiner, 1, 1);
    SaveTree(output, tree->leftChild, bitContatiner);
    SaveTree(output, tree->rightChild, bitContatiner);

}

void PrintCodeFile(FILE* in, FILE* out, Code** codeTable, BitContainer* bitContainer) {
    int c, i;
    while ((c = fgetc(in)) != EOF) {
        for (i = 0; i < codeTable[c]->size / CHAR_BIT; i++) {
            WriteBits(out, bitContainer, CHAR_BIT, codeTable[c]->code[i]);
        }
        WriteBits(out, bitContainer, codeTable[c]->size % CHAR_BIT, codeTable[c]->code[i]);
    }
    if (bitContainer->size) {
        fputc(bitContainer->symb << (CHAR_BIT - bitContainer->size), out);
    }
}

void MakeOutput(HuffTree* tree, Code** codeTable, FILE* input, FILE* output) {
    BitContainer* bitContainer = NewBitContainer(); //ontainer on 8 bits
    if (tree)
        fwrite(&tree->count, sizeof(unsigned int), 1, output);
    SaveTree(output, tree, bitContainer);
    PrintCodeFile(input, output, codeTable, bitContainer);

    free(bitContainer);
}

void EncodeFile(FILE* input, FILE* output) {
    int* symbTable;
    symbTable = NewIntArray((int)LETTER_COUNT);
    FillIntArray(symbTable, input);

    HuffTree** trees;
    trees = NewTreesArray();
    MakeTree(trees, symbTable);
    free(symbTable);

    Code** codeTable = NewCodeArray(LETTER_COUNT);
    BuildCodeTable(codeTable, trees[0], NewCodeElement(NULL, 0));

    MakeOutput(trees[0], codeTable, input, output); //Huffman tree is in the first element of trees array
    RemoveCodeArray(codeTable, LETTER_COUNT);

    RemoveTreesArray(trees);
}

uchar GetBit(FILE* input, BitContainer* bitContainer) {
    if (!bitContainer->size) {
        bitContainer->symb = (uchar)fgetc(input);
        bitContainer->size = CHAR_BIT;
    }
    uchar symb = bitContainer->symb >> (bitContainer->size - 1);
    bitContainer->size--;
    return symb & 1;

}

uchar GetBits(FILE* input, BitContainer* bitContainer, int count) {
    uchar symb = 0;
    for (int i = 0; i < count; i++) {
        symb = (symb << 1) + GetBit(input, bitContainer);
    }
    return symb;
}

HuffTree* GetCodeTree(FILE* input, BitContainer* bitContainer) {
    HuffTree* tree;
    tree = NewTree();
    uchar bit = GetBit(input, bitContainer);
    if (bit == 0) {
        uchar symb = GetBits(input, bitContainer, CHAR_BIT);
        InitTree(tree, symb, 0, 0, 0);
        return tree;
    }
    else {
        HuffTree* left = GetCodeTree(input, bitContainer);
        HuffTree* right = GetCodeTree(input, bitContainer);
        InitTree(tree, 0, 0, left, right);
        return tree;
    }

}

void DecodeSymb(FILE* input, FILE* output, HuffTree* tree, BitContainer* bitContainer) {
    if (tree->leftChild && tree->rightChild) {
        uchar bit = GetBit(input, bitContainer);
        if (bit == 0) {
            DecodeSymb(input, output, tree->leftChild, bitContainer);
        }
        else {
            DecodeSymb(input, output, tree->rightChild, bitContainer);
        }
    }
    else {
        fputc(tree->symb, output);
    }

}


void DecodeInput(FILE* input, FILE* output, HuffTree* tree, BitContainer* bitContainer, int size) {
    for (int i = 0; i < size; i++) {
        DecodeSymb(input, output, tree, bitContainer);
    }
}

void DecodeFile(FILE* input, FILE* output) {
    HuffTree* tree;
    BitContainer* bitContainer;
    unsigned int size = 0;
    if (!fread(&size, sizeof(unsigned int), 1, input))
        return;

    bitContainer = NewBitContainer();
    tree = GetCodeTree(input, bitContainer);
    DecodeInput(input, output, tree, bitContainer, size);
    RemoveTree(tree);
    free(bitContainer);
}

int main(void) {
    FILE* input = fopen("in.txt", "rb");
    assert(input);
    FILE* output = fopen("out.txt", "wb");
    assert(output);

    uchar mode;
    mode = (uchar)fgetc(input);
    fseek(input, 3, SEEK_SET);
    /*two modes: c = code data, d = decode data*/
    if (mode == 'c') {
        EncodeFile(input, output);
    }
    if (mode == 'd')
        DecodeFile(input, output);

    fclose(input);
    fclose(output);
    return 0;
}
