// COMP2521 ASS1 22T1: PART 2 HELPER FUNCTIONS HEADER FILE
// BY z5363412
// Written 25/03/22

#ifndef PART_2_HELPER_FUNCTION_C
#define PART_2_HELPER_FUNCTION_C
#include <stdio.h>
#include <stdbool.h>

#include "invertedIndex.h"
//////////////////////////////////////////////////////////////////////////////

// Searches the tree for the word
InvertedIndexBST findWord(InvertedIndexBST indexTree, char *str);

// Calculates the TfIdf
double calculateTfIdf(FileList curr, int D);

// Creates a new node for the new linked list
TfIdfList newTfIdfNode (char *filename, double tf, double idf);

// Inserts into new list
TfIdfList insertList(TfIdfList curr, TfIdfList node);

//////////////////////////////////////////////////////////////////////////////

// Inserts nodes into list for certain instances
TfIdfList createSearchList(TfIdfList word, TfIdfList list);

// Checks if the file is a duplicate
bool duplicate(TfIdfList list, char *filename);

// Creates new node
TfIdfList newListNode (char *filename, double tfIdfSum);

// Sorts the list 
void sort(TfIdfList list);

//////////////////////////////////////////////////////////////////////////////
 
#endif