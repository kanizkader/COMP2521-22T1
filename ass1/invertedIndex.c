// COMP2521 ASS1 22T1: INVERTED INDEX FUNCTIONS
// BY z5363412
// Written 21/03/22

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "invertedIndex.h"
#include "part1HelperFunction.h"
#include "part2HelperFunction.h"

#define MAX_WORD 100
//////////////////////////////////////////////////////////////////////////////

// Part 1

InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
	
	// Create tree
	InvertedIndexBST indexTree = NULL;

    // Open file to insert the words 
    char *fileName = malloc(MAX_WORD * sizeof(char));
    char *word = malloc(MAX_WORD * sizeof(char));

	FILE *primaryFile = fopen(collectionFilename, "r");

    while (fscanf(primaryFile, "%s", fileName) != EOF) {

        FILE *secondaryFile = fopen(fileName, "r");
        while (fscanf(secondaryFile, "%s", word) != EOF) {

            word = normaliseWord(word);
			// Check that it's not an empty word
			if (word != NULL) {
            	indexTree = insertWord(indexTree, fileName, word);
			}

        }
        fclose(secondaryFile);
    }
    fclose(primaryFile); 

    free(fileName);
    free(word);   
    return indexTree;
}

void printInvertedIndex(InvertedIndexBST tree, char *filename) {
    if (tree == NULL) {
        return;
    }

    FILE *file = fopen(filename, "w+");        
    printTree(tree, file);    
    fclose(file);
    return;
}

void freeInvertedIndex(InvertedIndexBST tree) {
    if (tree == NULL) {
        return;
    }
    freeWords(tree);    
}

//////////////////////////////////////////////////////////////////////////////

// Part 2

TfIdfList searchOne(InvertedIndexBST tree, char *searchWord, int D) {
    TfIdfList list = NULL;

   // Find the given word in the tree
    InvertedIndexBST wantedWord = findWord(tree, searchWord);
    if (wantedWord == NULL) {
        return list;
    }

    FileList wantedList = wantedWord->fileList;
    double idf = calculateTfIdf(wantedList, D);

    // Insert filelist into new list
    while (wantedList != NULL) {
        TfIdfList newNode = newTfIdfNode(wantedList->filename, wantedList->tf, idf);
        list = insertList(list, newNode);
        wantedList = wantedList->next;
    }    
    return list;
}

TfIdfList searchMany(InvertedIndexBST tree, char *searchWords[], int D) {
    // Get the num of search words
    int searchWordsLen = 0;
    while (searchWords[searchWordsLen] != NULL) {
        searchWordsLen++;
    }

    TfIdfList list = NULL;
    if (searchWordsLen == 0) {
        return list;
    }
    
    // For each search words, insert filelist into new list
    int i = 0;
    while (i < searchWordsLen) {
        InvertedIndexBST wantedWord = findWord(tree, searchWords[i]);
        if (wantedWord == NULL) {
            i++;
            continue;
        }

        TfIdfList word = searchOne(wantedWord, searchWords[i], D);
        if (word == NULL) {
            i++;
            continue;
        }

        list = createSearchList(word, list);
        i++;
    }
    sort(list);
    return list;
}

void freeTfIdfList(TfIdfList list) {
    if (list == NULL) {
        return;
    }

    while (list != NULL) {
        free(list->filename);
        TfIdfList tmp = list->next;
        free(list);
        list = tmp;
    }
    return;
}

//////////////////////////////////////////////////////////////////////////////

