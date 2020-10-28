#include <stdio.h>
#include <stdlib.h> // for MALLOC()!!!
#include <string.h> 
#include <ctype.h>
#include "tree.h"

// global var holding the number of the most frequent word
int mostFreq = 1;

// BST word node constructor
struct node* newWord(char* word) { 
    struct node* temp =  (struct node*) malloc(sizeof(struct node)); 
    if (temp == NULL) {
        return NULL;
    }
    temp->word = word; 
    temp->freq = 1; // if you create a new word, it would have appeared once
    temp->left = NULL;
    temp->right = NULL; 
    return temp; 
} 

// goal: free all nodes of BST when done.
// param root: struct node* of BST.
// return void.
void postOrderFree(struct node* root) {
    if (root != NULL) {
        postOrderFree(root->left);
        postOrderFree(root->right);
        free(root->word);
        free(root);
        root = NULL;
    }
}

// goal: helper function to inorder(). will print out to stdin (or stderr) with write().
// param root: struct node* to represent root node.
// param longest: int representing length of longest int for formatting.
// param  fd: int file descriptor of file we will write to.
// return: void
void inorderHelper(struct node* root, int longest, int fd) {  
    if (root != NULL) { 
        // recur down left subtree
        inorderHelper(root->left, longest, fd); 
        // print out current word (if it isn't empty string)
        if(strlen(root->word) != 0){
            char buffer[longest + 3 + root->freq];
            sprintf(buffer, "%-*s :%*i\n", longest, root->word, (int)(log10(mostFreq) + 2), root->freq);
            if (write(fd, buffer, strlen(buffer)) == -1) {
                char err[1024];
                sprintf(err, "Error in writing to output: %i\n", errno);
                if (write(2, err, strlen(err)) == -1) {
                    perror("Error in writing to output.txt.\n");
                }
            }
        }
        // recur down right subtree
        inorderHelper(root->right, longest, fd); 
    } 
} 

// goal: traverses 26 BSTs inorder.
// param root[]: struct node* array to represent roots 26 BSTs.
// param longest: int representing length of longest int for formatting.
// return: void
void inorder(struct node* root[], int longest) { 
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        char* err = "Cannot open output.txt";
        if (write(2, err, strlen(err)) == -1) {
            perror("Cannot open output.txt.\n");
        }
    }
    for (int i = 0; i < 26; i++) {
        // printf("index %i\n", i);
        if (root[i] != NULL) { 
            //printf("root[i] word: %sa\n", root[i]->word);
            inorderHelper(root[i], longest, fd);  
        } 
    }
    close(fd);
} 

// goal: insert a given root and a string word to insert.
// param node: struct node* to represent a subtree node.
// param word: char* representing a string to insert.
// return: node who had data inserted into.
struct node* insert(struct node* node, char* word) { 
    /* If the tree is empty, return a new node */
    if (node == NULL) {
        // found null spot. inserting word
        char* copied =  (char*) malloc(strlen(word));
        if (copied == NULL) {
            char* err = "No more memory for malloc during BST insert.\n";
            if (write(2, err, strlen(err)) == -1) {
                perror("No more memmory for malloc during BST insert.\n");
            }
        }
        strcpy(copied, word);
        node = newWord(copied);
        return node; 
    }
    // Otherwise, recur down the tree 
    int j = strcmp(word, node->word);
    
    // recur down left 
    if (strcmp(word, node->word) < 0) {
        node->left  = insert(node->left, word); 
    }
    // recur down right
    else if (strcmp(word, node->word) > 0) {
        node->right = insert(node->right, word); 
    } else {
        // duplicate word found, increment frequency
        node->freq = ++node->freq;
        if (node->freq > mostFreq) {
            mostFreq = node->freq;
        }
    }   
    // return unchanged node
    return node; 
} 

// goal: hash function that returns the index in the BST array of first letter of inputed word
// param word: char* that represents the word to be inserted.
// return: int, index in BST array.
int hash(char* word) {
    char first = word[0];
    // mod by 97 because lowercase 'a' is 97.
    return first % 97;
}

// goal: takes in a words buffer, parses each word, and inserts it into BST array. 
    // returns length of longest word found.
// param words[]: char array of buffer from previous read() call.
// param count: length of words buffer. 
// return: int length of longest word.
int parseWord(char words[], int count, struct node* dictionary[]) {
    // initialize word search variables
    int wordsize = 7;
    char* iword = (char*) malloc(wordsize);
    if (iword == NULL) {
        char* err = "No more memory for malloc during parseWord.\n";
        // write error to stderr 2
        if (write(2, err, strlen(err)) == -1) {
            perror("No more memory for malloc during parseWord.\n");
        }
    }
    int wordlength = 0;
    int longest = 0;
    for (int i = 0; i < count + 1; i++) {
        // case: not alpha char found, insert into BST.
        if (!isalpha(words[i])) {
            // space found
            if (strcmp(iword, "") != 0) {
                int letter = hash(iword);
                iword[wordlength] = '\0';
                // if root of BST is null, set root.
                if (dictionary[letter] == NULL) {
                    dictionary[letter] = insert(dictionary[letter], iword);
                } else {
                    // otherwise recursively insert.
                    insert(dictionary[letter], iword);
                }
            } 
            // set longest word length.
            if (wordlength > longest) {
                longest = wordlength;
            }
            // reset word search variables.
            wordlength = 0;
            strcpy(iword, ""); // clears buffer.
            if (words[i] == '\0') {
                // Done parsing words buffer. 
                break;
            }
        } 
        // letter found, add to word
        else if (isalpha(words[i])) {
            if (wordlength == wordsize) { 
                // wordlength about to exceed wordsize: we need to realloc.
                wordsize = wordsize * 2;
                // wordsize doubled to accommodate bigger, badder words.
                iword = (char*) realloc(iword, wordsize);
            } 
            // concat a single letter to temp word buffer iword.
            char letter = tolower(words[i]);
            strncat(iword, &letter, 1);
            wordlength++;
        }
    } 
    free(iword);
    return longest;
}