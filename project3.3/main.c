#include <stdio.h>
#include <stdlib.h> // for MALLOC()!!!
#include <sys/types.h> // open()
#include <fcntl.h> // open()
#include <unistd.h> // read(), close()
#include <errno.h> // for errno
#include <sys/stat.h>
#include "tree.h"

// extern char** environ;

// goal: function to read a file.
// param filename: char* representing name of file to be read.
// param dictionary: struct node array BST.
// return: int length of longest word.
int readFile(char* filename, struct node* dictionary[]) {
    int fdin;
    int temp;
    if ((fdin = open(filename, O_RDONLY)) < 0) { 
        // "continue gracefully"
        char* err = "A file failed to open. Ignoring. \n";
        if (write(2, err, strlen(err)) == -1) {
            // incase write fails.
            perror("A file failed to open. Ignorning. \n");
        }
        return 0;
    }
    // find size of file
    off_t start = lseek(fdin, (size_t) 0, SEEK_CUR); // sets an off_t to the beginning
    off_t BUFFER_SIZE = lseek(fdin, (size_t) 0, SEEK_END); // sets offset to end
    lseek(fdin, start, SEEK_SET); // must set it back to beginning so we can read()
    // recreate file as a string
    char* words = (char*) malloc(BUFFER_SIZE + 1);
    if (words == NULL) {
        char* err = "No more memory for malloc during file read!!!\n";
        if (write(2, err, strlen(err)) == -1) {
            // writing to stderr failed.
            perror("No more memory for malloc during file read!!!");
        }
    }
    words[BUFFER_SIZE] = '\0';
    // read file into string buffer words.
    ssize_t cnt = read(fdin, words, BUFFER_SIZE);
    // error in reading.
    if (cnt == 0) {
        char* err = "Failed to read a file.\n";
        if (write(2, err, strlen(err)) == -1) {
            // writing to stderr failed.
            perror("Failed to read a file.\n");
        }
        return 0;
    }
    // no error in reading, parse the buffer.
    else if (cnt > 0) {
        temp = parseWord(words, cnt, dictionary);
    }
    // close the file.
    int success = close(fdin);
    if (success == -1) {
        char* error = "A file cannot be closed. Ignoring. \n";
        if (write(2, error, strlen(error)) == -1) {
            // writing to stderr failed.
            perror("A file cannot be closed. Ignoring. \n");
        }
    } 
    // return length of longest word.
    return temp;
}

// main function
int main(int argc, char* argv[]) {
    // initialize array of 26 BSTs for letters 'a' to 'z'.
    struct node* dictionary[] = {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  
    };
    // length of longest word.
    int longest = 0;

    // Argv case: loop through argc for argv files.
    for (int i = 1; i < argc; ++i) {
        int temp = readFile(argv[i], dictionary);
        if (temp > longest) {
            longest = temp;
        }
    }

    // Environment Variable case: checking for 'WORD_FREAK'.
    char* enval = getenv("WORD_FREAK");
    if (enval != NULL) {
        // read file and store file's longest word.
        int temp = readFile(enval, dictionary);
        if (temp > longest) {
            longest = temp;
        }
    }

    // STDIN case: reading in from standard input.
    int BUFFER_SIZE = 1024;
    char letter[BUFFER_SIZE + 1];
    // wordsize is temporary max size of buffer.
    int wordsize = BUFFER_SIZE;
    // We will parse all of the user's input into this string.
    char* rebuiltStr = (char*) malloc(wordsize + 1);
    if (rebuiltStr == NULL) {
        char* err = "No more memory!!!";
        write(2, err, strlen(err));
    }
    // temp counter that increments.
    int wordlength = 0;
    // malloc letter and free it every time.
    ssize_t cnt;
    int i = 0;
    // read from stdin 
    while (((cnt = read(0, letter, BUFFER_SIZE)) > 0)) {
        if (wordlength == wordsize) {     
            // wordlength about to exceed wordsize: we need to realloc.
            wordsize = wordsize + BUFFER_SIZE;
            // wordsize doubled to accommodate bigger, badder words.
            rebuiltStr = (char*) realloc(rebuiltStr, wordsize);
            if (rebuiltStr == NULL) {
                char* err = "Cannot realloc.\n";
                if (write(2, err, strlen(err)) == -1) {
                    perror("Cannot realloc.\n");
                }
            }  
        }
        // concat. the buffer onto the string.
        strncat(rebuiltStr, letter, cnt);
        wordlength += cnt;
        i++;
    }
    // error with read.
    if (cnt == -1) {
        char* err = "Failed to read from STDIN.\n";
        if (write(2, err, strlen(err)) == -1) {
            perror("Failed to read from STDIN.\n");
        }
    } else {
        // rebuiltStr is now STDIN as a string that we can parseWords on.
        rebuiltStr[wordlength] = '\0';
        int temp = parseWord(rebuiltStr, wordlength, dictionary); 
        if (temp > longest) {
            longest = temp;
        } 
    }
    
    free(rebuiltStr);
    rebuiltStr = NULL;
    // inorder traverse BST to write it alphabetically to a file.
    inorder(dictionary, longest);
    // free all nodes in tree with post-order traversal.
    for (int i = 0; i < 26; i++) {
        postOrderFree(dictionary[i]);
        dictionary[i] = NULL;
    }
    return 0;
}