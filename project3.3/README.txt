Hello!!!

    Link to Youtube Demo: https://youtu.be/zKPrXHWKiNE
    My implementation has 3 files: tree.h, tree.c, and main.c. All functions related to reading input is in main.c,
and all functions and data structures related to the BST are defined in tree.h and tree.c. 
    BST requirement: 
        - The BST "dictionary" is initially declared as an array of 26 NULLs in main() in main.c. 
    As we read from argv, environment variables, and STDIN, the input will be parsed with parseword(). 
    parseWord() will create a tree node, hash() to figure out at which index it must be inserted. 
    Finally, we will insert the word node into the tree with insert(), which follows regular BST insertion rules.
    Hash requirement: 
        - My program decides where in the tree to place it with the hash() function in tree.c, which returns
    the ascii value/index in the array. We mod(%) the first character in the word string by 97(which is ascii lowercase a),
    and this returns the ascii value of the first letter, which is also the index in the BST of 26.

Files and details into the functions:
    - main.c:
        - main(): 
            - initializes BST, calls readFile() for handling argv and environment variables,
            and read()s STDIN by rebuilding input as a malloc'd string to pass into parseWords().
        - readFile(): 
            - takes in a char* denoting name of file, open()s it
            and read()s contents of file into lseek()-sized buffer. String buffer can now go to parseWords.
    - tree.h:
        - struct node:
            - tree node struct is defined here with a freq for frequency, word for string, left, 
            and right pointers.
    - tree.c:
        - insert(), inorder(), inorderhelper():
            - inserting into, traversing, and outputting the BST takes place here. 
        - parseWords(): 
            - takes a string and parses each letter into words with isalpha() and tolower(). 
            When a word is found, we insert it into the dictionary BST. Returns the length of the longest word. 
        - insert(), inorder(): 
            - are regular BST operations. If a word already exists in tree, we increment frequency. 
        - inorderhelper(): 
            - properly formats and writes into output.txt. We properly format by keeping track of the longest
            word(paramenter) and the most frequent word(global variable).

