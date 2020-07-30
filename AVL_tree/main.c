#include <stdlib.h>
#include <stdio.h>

/*Data structure AVL tree;
Task: implement the data structure, find the height of the tree
*/

typedef struct AvlTree {
   int height;
   int data;
   struct AvlTree * leftChild;
   struct AvlTree * rightChild;
}
AvlTree;

int getHeight(AvlTree * tree) {
   if (tree)
      return tree->height;
   else
      return 0;
}

int getBalanceFactor(AvlTree * tree) {
   return getHeight(tree->rightChild) - getHeight(tree->leftChild);
}

void installHeight(AvlTree * tree) {
   if (getHeight(tree->leftChild) > getHeight(tree->rightChild)) {
      tree->height = getHeight(tree->leftChild) + 1;
   } else tree->height = getHeight(tree->rightChild) + 1;
}

AvlTree * turnLeft(AvlTree * tree) {
   AvlTree * saveTree = tree->rightChild;
   tree->rightChild = saveTree->leftChild;
   saveTree->leftChild = tree;
   installHeight(tree);
   installHeight(saveTree);
   return saveTree;

}

AvlTree * turnRight(AvlTree * tree) {
   AvlTree * saveTree = tree->leftChild;
   tree->leftChild = saveTree->rightChild;
   saveTree->rightChild = tree;
   installHeight(tree);
   installHeight(saveTree);

   return saveTree;

}

AvlTree * balance(AvlTree * tree) {
  installHeight(tree);

   if (getBalanceFactor(tree) == 2) {
      if (getBalanceFactor(tree->rightChild) < 0)
         tree->rightChild = turnRight(tree->rightChild);
      return turnLeft(tree);
   }

   if (getBalanceFactor(tree) == -2) {
      if (getBalanceFactor(tree->leftChild) > 0) {
         tree->leftChild = turnLeft(tree->leftChild);
      }
      return turnRight(tree);
   }

   return tree;

}

AvlTree * newTree(int data) {

   AvlTree * tree = (AvlTree * )malloc(sizeof(AvlTree));
   tree->data = data;
   tree->leftChild = NULL;
   tree->rightChild = NULL;
   tree->height = 1;
   return tree;

}

AvlTree * push(AvlTree * tree, int data) {
   if (!tree) return newTree(data);

   if (tree->data > data)
      tree->leftChild = push(tree->leftChild, data);
   else
      tree->rightChild = push(tree->rightChild, data);
   return balance(tree);
}

void deleteTree( AvlTree** tree) {
    if(*tree) {
        deleteTree(&(*tree)->leftChild);
        deleteTree(&(*tree)->rightChild);
        free((*tree));
    }
}


int main(void) {
   int countOfVertex = 0;
   int data = 0;
   AvlTree * tree;

   if(!scanf("%d", & countOfVertex)) return 0;
   if (countOfVertex == 0) {
      printf("%d", 0);
      return 0;
   }

   if(!scanf("%d", & data)) return 0;
   tree = newTree(data);
   for (int i = 1; i < countOfVertex; i++) {
      if(!scanf("%d", & data)){ deleteTree(&tree); return 0;}
      tree = push(tree, data);
   }

   printf("%d", tree->height);
   deleteTree(&tree);
   return 0;
}
