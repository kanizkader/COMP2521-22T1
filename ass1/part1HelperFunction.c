// COMP2521 ASS1 22T1: PART 1 HELPER FUNCTIONS
// BY z5363412
// Written 23/03/22

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "part1HelperFunction.h"
#define MAX_WORD 100
//////////////////////////////////////////////////////////////////////////////

// Declarations

// creates new node for tree
static InvertedIndexBST newBST(char *filename, char *str); 

// creates new node for file list
static FileList newNode(char *filename, double tf); 

//////////////////////////////////////////////////////////////////////////////

// generateInvertedIndex Helper Functions

char *normaliseWord(char *str) { 

    // Make string lowercase
	int i = 0;
    while (str[i] != '\0') {
        str[i] = tolower(str[i]);
        i++;
    }

	// Go to the last char in str
	i -= 1;
    // Remove . , ; ?
	while (i != 0) {
		if (str[i] == '.' 
        || str[i] == ',' 
        || str[i] == ';' 
        || str[i] == '?') 
        {
			str[i] = '\0';
		} else {
			break;
		}
		i -= 1;
	}
    return str;
}

static InvertedIndexBST newBST(char *filename, char *str) {
    InvertedIndexBST node = malloc(sizeof(struct InvertedIndexNode));
    node->word = malloc((MAX_WORD + 1) * sizeof(char));
    strcpy(node->word, str);
    node->fileList = NULL;
    node->right = node->left = NULL;
    insertFile(node, filename, str);
    return node;
}

InvertedIndexBST insertWord(InvertedIndexBST indexTree, 
                            char *filename, char *str) 
{
    if (indexTree == NULL) { // If tree is empty       
        return newBST(filename, str);
    } 
    
    if (strcmp(indexTree->word, str) == 0) { 
        insertFile(indexTree, filename, str);
    } else if (strcmp(indexTree->word, str) > 0) { 
        indexTree->left = insertWord(indexTree->left, filename, str);
    } else if (strcmp(indexTree->word, str) < 0) { 
        indexTree->right = insertWord(indexTree->right, filename, str);
    }
    return indexTree;
}

static FileList newNode(char *filename, double tf) {
    FileList node = malloc(sizeof(struct FileNode));
    node->filename = malloc((MAX_WORD + 1) * sizeof(char));
    strcpy(node->filename, filename);
    node->next = NULL;
    node->tf = tf;
    return node;
}

void insertFile(InvertedIndexBST treeNode, char *filename, char *word) {
    double tf = calculatetF(filename, word); 
    FileList node = newNode(filename, tf);
    FileList curr = treeNode->fileList;
    FileList prev;

    if (treeNode->fileList == NULL) {
        treeNode->fileList = node;
        return;
    }

    // check for dup
    if (strcmp(filename, curr->filename) == 0) {
        free(node->filename);
        free(node);
        return;
    }

    // insert at head
    if (strcmp(filename, curr->filename) < 0) {
        prev = curr;
        treeNode->fileList = node;
        node->next = prev;
        return; 
    }
    // check middle
    while (curr != NULL) { // file name, curr
        if (strcmp(filename, curr->filename) == 0) {
            free(node->filename);
            free(node); 
            return;
        } else if ((strcmp(filename, curr->filename) < 0) 
                    && (strcmp(filename, prev->filename) > 0)) {
            prev->next = node;
            node->next = curr;
            return;
        }
        prev = curr;
        curr = curr->next;

    }

    // end case
    if (curr == NULL) {
        prev->next = node;
    }
    
    return;

}

double calculatetF(char *filename, char *word) {
    FILE *file = fopen(filename, "r");    
    char *curr = malloc((MAX_WORD + 1) * sizeof(char));
    double freq = 0; 
    double num = 0; 
    
    while (fscanf(file, "%s", curr) != EOF) {
        curr = normaliseWord(curr);
        if (strcmp(curr, word) == 0) {
            freq++;
            num++;
            continue;
        } 
        num++;        
    }
    double tf = freq / num;
    free(curr);
    fclose(file);
    return tf;
}

//////////////////////////////////////////////////////////////////////////////

// printInvertedIndex Helper Functions

void printTree(InvertedIndexBST tree, FILE *file) {
    if (tree == NULL) {
        return;
    }
    printTree(tree->left, file);
    fprintf(file, "%s ",tree->word);
    FileList curr = tree->fileList;
    while (curr != NULL) {
        fprintf(file,"%s (%.7lf) ",curr->filename, curr->tf);
        curr = curr->next;
    }
    fprintf(file,"\n");
    printTree(tree->right, file);
}

//////////////////////////////////////////////////////////////////////////////

// freeInvertedIndex Helper Functions
void freeWords(InvertedIndexBST tree) {
    if (tree == NULL) {
        return;
    }
    freeWords(tree->left);
    freeFileList(tree->fileList);
    free(tree->word);
    freeWords(tree->right);
    free(tree);
}

void freeFileList(FileList list) {
    while (list != NULL) {
        free(list->filename);
        FileList tmp = list->next;
        free(list);
        list = tmp;
    }
    return;
}
//////////////////////////////////////////////////////////////////////////////
