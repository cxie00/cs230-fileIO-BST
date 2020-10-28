#include <stdio.h>
#include <stdlib.h> // for MALLOC()!!!
#include <string.h> 
#include <sys/types.h> // open()
#include <fcntl.h> // open()
#include <unistd.h> // read(), close()
#include <errno.h> // for errno
#include <sys/stat.h>
#include <math.h>
#include <errno.h>

// node constructor
struct node { 
    char* word; 
    int freq; 
    struct node *left;
    struct node *right; 
}; 

// BST word node constructor
struct node *newNode(char* word);

// goal: free all nodes of BST when done.
// param root: struct node* of BST.
// return void.
void postOrderFree(struct node* root);

// goal: helper function to inorder(). will print out to stdin (or stderr) with write().
// param root: struct node* to represent root node.
// param longest: int representing length of longest int for formatting.
// param  fd: int file descriptor of file we will write to.
// return: void
void inorderHelper(struct node* root, int longest, int fd);

// goal: traverses 26 BSTs inorder.
// param root[]: struct node* array to represent roots 26 BSTs.
// param longest: int representing length of longest int for formatting.
// return: void
void inorder(struct node* root[], int longest);

// goal: insert a given root and a string word to insert.
// param node: struct node* to represent a subtree node.
// param word: char* representing a string to insert.
// return: node who had data inserted into.
struct node* insert(struct node* node, char* word);

// goal: hash function that returns the index in the BST array of first letter of inputed word
// param word: char* that represents the word to be inserted.
// return: int, index in BST array.
int hash(char* word); 

// goal: takes in a words buffer, parses each word, and inserts it into BST array. 
    // returns length of longest word found.
// param words[]: char array of buffer from previous read() call.
// param count: length of words buffer. 
// return: int length of longest word.
int parseWord(char words[], int count, struct node* dictionary[]);