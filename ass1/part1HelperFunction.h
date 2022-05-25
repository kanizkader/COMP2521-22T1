// COMP2521 ASS1 22T1: PART 1 HELPER FUNCTIONS HEADER FILE
// BY z5363412
// Written 23/03/22

#ifndef PART_1_HELPER_FUNCTION_C
#define PART_1_HELPER_FUNCTION_C
#include <stdio.h>

#include "invertedIndex.h"
///////////////////////////////////////////////////////////////////////////////

// Normalises the word by making it lowercase and remove any punctuation
// off the end.
char *normaliseWord(char *str);

// Creates a the BST tree
InvertedIndexBST insertWord(InvertedIndexBST indexTree, 
                            char *filename, 
                            char *str);

// Inserts files into the linked list alphabetically, 
// whilst also checking for duplicates
void insertFile(InvertedIndexBST treeNode, char *filename, char *word);

// Calculates the tF
double calculatetF(char *filename, char *word);

//////////////////////////////////////////////////////////////////////////////

// Prints tree to a specific file
void printTree(InvertedIndexBST tree, FILE *file); 

//////////////////////////////////////////////////////////////////////////////

// Frees the tree nodes
void freeWords(InvertedIndexBST tree);

// Frees the linked list nodes
void freeFileList(FileList list);

//////////////////////////////////////////////////////////////////////////////
 
#endif