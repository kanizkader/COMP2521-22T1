// COMP2521 ASS1 22T1: PART 2 HELPER FUNCTIONS
// BY z5363412
// Written 25/03/22

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "part2HelperFunction.h"

#define MAX_WORD 100
//////////////////////////////////////////////////////////////////////////////

// Declarations

static void swap(TfIdfList a, TfIdfList b);

//////////////////////////////////////////////////////////////////////////////

// SearchOne Helper Functions

InvertedIndexBST findWord(InvertedIndexBST indexTree, char *str) {
    // Traverse tree recursively
    if (indexTree == NULL) {
        return NULL;
    } else if (strcmp(indexTree->word, str) == 0) {
        return indexTree;
    } else if (strcmp(indexTree->word, str) < 0) {
        return findWord(indexTree->right, str);
    }    
    return findWord(indexTree->left, str);    
}

double calculateTfIdf(FileList curr, int D) {
    // Count num of documents
    double documents = 0;
    while (curr != NULL) {
        documents++;
        curr = curr->next;
    }
    return log10(D / documents);
}

TfIdfList newTfIdfNode (char *filename, double tf, double idf) {
    TfIdfList newNode = malloc(sizeof(struct TfIdfNode));
    newNode->filename = malloc((MAX_WORD + 1) * sizeof(char));
    strcpy(newNode->filename, filename);
    newNode->tfIdfSum = (tf * idf);
    newNode->next = NULL;
    return newNode;
}

TfIdfList insertList(TfIdfList curr, TfIdfList node) {
    if (curr == NULL) {
        return node;
    } 
    
    // Insert into new list
    if (node->tfIdfSum == curr->tfIdfSum) {
        if (strcmp(node->filename, curr->filename) > 0) { 
            node->next = curr->next;
            curr->next = node;
        } else if (strcmp(node->filename, curr->filename) < 0) {
            node->next = curr;
            return node;
        } 
        return curr;        
    } else if (node->tfIdfSum > curr->tfIdfSum) { 
        node->next = curr;
        return node;
    } 
    curr->next = insertList(curr->next, node);     
    return curr;
}

//////////////////////////////////////////////////////////////////////////////

// SearchMany Helper Functions

TfIdfList createSearchList(TfIdfList word, TfIdfList list) {
    if (list == NULL) {
        list = word;
        return list;
    }
    
    while (word != NULL) {
        TfIdfList curr = list;

        if (duplicate(curr, word->filename)) {
            while (strcmp(curr->filename, word->filename) != 0) {
                curr = curr->next;
            }
            curr->tfIdfSum = curr->tfIdfSum + word->tfIdfSum;
        } else {
            while (curr->next != NULL) {
                curr = curr->next;
            }
            curr->next = newListNode(word->filename, word->tfIdfSum);
        }
        word = word->next;
    }
    
    return list;
}

bool duplicate(TfIdfList list, char *filename) {
    TfIdfList curr = list;
    while (curr != NULL) {
        if (strcmp(curr->filename, filename) == 0) {
            return true; 
        }
        curr = curr->next;
    }
    return false;
}

TfIdfList newListNode (char *filename, double tfIdfSum) {
    TfIdfList newNode = malloc(sizeof(struct TfIdfNode));
    newNode->filename = malloc((MAX_WORD + 1) * sizeof(char));
    strcpy(newNode->filename, filename);
    newNode->tfIdfSum = tfIdfSum;
    newNode->next = NULL;
    return newNode;
}

void sort(TfIdfList list) {
    // Empty List or only one item in list
    if (list == NULL) {
        return;
    }
    
    // Bubble sorts the list
    int swapped = false;
    do {
        swapped = false;
        TfIdfList curr = list;

        while (curr->next != NULL) {
            if (curr->tfIdfSum == curr->next->tfIdfSum) {
                if (strcmp(curr->filename, curr->next->filename) > 0) {
                    swap(curr, curr->next);
                    swapped = true;
                }
            } else if (curr->tfIdfSum < curr->next->tfIdfSum) {
                swap(curr, curr->next);
                swapped = true;
            }
            curr = curr->next;
        }
    }
    while (swapped);
}

static void swap(TfIdfList a, TfIdfList b) {
    char *temp = a->filename;
    double temp2 = a->tfIdfSum;
    a->filename = b->filename;
    a->tfIdfSum = b->tfIdfSum;
    b->filename = temp;
    b->tfIdfSum = temp2;
    
}

//////////////////////////////////////////////////////////////////////////////
