#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

uint32_t numWords = 0;
uint32_t numDownWords = 0;

/* This struct defines characteristics of words that are present in the crossword.
 * row and col give position of first letter of word
 * length is the length of the word
 * orientation if whether the word is horizontal (across) or vertical (down)
 */
typedef struct word{
    uint32_t row;
    uint32_t col;
    uint32_t length;
    char* word;
    char* orientation;
} word;


/*
 * this function allocates memory on heap for pointers to word structs
 */
word** createWordList(uint32_t dimensions[]){
    uint32_t rows = dimensions[0];
    uint32_t cols = dimensions[1];
    word** wordlist = (word**) malloc((rows*cols) * sizeof(word*));
    if (wordlist == NULL){
        printf("Memory could not be allocated for word list\n");
        abort();
    }

    return wordlist;
}

/*
 * This function creates word struct itself on heap, assigning values to characteristics.
 */
void buildWord(uint32_t row, uint32_t col, uint32_t length, char* orientation, char* tempword, word** wordlist){
    wordlist[numWords] = (word*) malloc(2*sizeof(char*) + 3 * sizeof(uint32_t));
    if (wordlist[numWords] == NULL){
        printf("Memory could not be allocated for word struct\n");
        abort();
    }
    wordlist[numWords]->row = row;
    wordlist[numWords]->col = col;
    wordlist[numWords]->length = length;
    wordlist[numWords]->word = (char*) malloc(length*sizeof(char));
    if (wordlist[numWords]->word == NULL){
        printf("Memory could not be allocated for the word string\n");
        abort();
    }
    wordlist[numWords]->word = strcpy(wordlist[numWords]->word, tempword);
    wordlist[numWords]->orientation = (char*) malloc(7*sizeof(char));
    if (wordlist[numWords]->orientation == NULL){
        printf("Memory could not be allocated for orientation string\n");
        abort();
    }
    strcpy(wordlist[numWords]->orientation, orientation);
}

/*
 * This function allocates all horizontal words in word struct on heap
 */
void getAcrossWords(uint32_t dimensions[], char** rowPtr, word** wordlist) {
    uint32_t rows = dimensions[0];
    uint32_t cols = dimensions[1];
    uint32_t k = 0;
    char tempword[cols];
    for (uint32_t i = 0; i < rows; i++) {
        uint32_t j = 0;
        for (j = 0; j < cols; j++) {
            if ((rowPtr[i][j] == '*') ) {
                    tempword[k] = 0;
                    if (k > 1) {
                        buildWord(i, (j-k), k, "across", tempword, wordlist);
                        //printf("%s, %d %d, %d\n", wordlist[numWords]->word, wordlist[numWords]->row, wordlist[numWords]->col, wordlist[numWords]->length );
                        //printf("%d\n",numWords);
                        numWords++;

                    }
                    k = 0;

            } else if ((j+1) == cols) {
                tempword[k] = rowPtr[i][j];
                k++;
                tempword[k] = 0;
                if (k > 1) {
                    buildWord(i, (j-k+1), k, "across", tempword, wordlist);
                    //printf("%s, %d %d, %d\n", wordlist[numWords]->word, wordlist[numWords]->row, wordlist[numWords]->col, wordlist[numWords]->length );
                    //printf("%d\n",numWords);
                    //printf("%s\n", wordlist[numWords]->word);
                    numWords++;
                }
                k=0;

            } else {
                    tempword[k] = rowPtr[i][j];
                    k++;
            }

        }

    }
}

/*
 * This function allocates all vertical words in word struct on heap
 */
void getDownWords(uint32_t dimensions[], char** rowPtr, word** wordlist) {
    uint32_t rows = dimensions[0];
    uint32_t cols = dimensions[1];
    uint32_t k = 0;
    char tempword[cols];
    for (uint32_t j = 0; j < cols; j++) {
        uint32_t i = 0;
        for (i = 0; i < rows; i++) {
            if ((rowPtr[i][j] == '*') ) {
                tempword[k] = 0;
                if (k > 1) {
                    buildWord((i-k), j, k, "down", tempword, wordlist);
                    //printf("%s, %d %d, %d\n", wordlist[numWords]->word, wordlist[numWords]->row, wordlist[numWords]->col, wordlist[numWords]->length );
                    //printf("%d\n",numWords);
                    numWords++;
                    numDownWords++;

                }
                k = 0;

            } else if ((i+1) == rows) {
                tempword[k] = rowPtr[i][j];
                k++;
                tempword[k] = 0;
                if (k > 1) {
                    buildWord((i-k+1), j, k, "down", tempword, wordlist);
                    //printf("%s, %d %d, %d\n", wordlist[numWords]->word, wordlist[numWords]->row, wordlist[numWords]->col, wordlist[numWords]->length );
                    //printf("%d\n",numWords);
                    //printf("%s\n", wordlist[numWords]->word);
                    numWords++;
                    numDownWords++;
                }
                k=0;

            } else {
                tempword[k] = rowPtr[i][j];
                k++;
            }

        }

    }
}

/*
 * frees all memory that was allocated by malloc in createWordList() and buildWord() functions
 */
void freeWordlist(word** wordlist){
    for(uint32_t i = 0; i < numWords; i++){
        free(wordlist[i]->word);
        free(wordlist[i]->orientation);
        free(wordlist[i]);
        wordlist[i]->word = NULL;
        wordlist[i]->orientation = NULL;
        wordlist[i] = NULL;

    }
    free(wordlist);
    wordlist = NULL;
}

/*
 * this function gets dimensions of crossword given in first line of text file
 */
void getDimensions(FILE* fptr, uint32_t dimensions[]) {
    for (uint32_t i = 0; i < 2; i++) {
        fscanf(fptr, "%d ", &dimensions[i]);
    }
}

/*
 * this function dynamically allocates memory on heap for rows of crossword
 */
char** allocateCrossword(uint32_t dimensions[]) {
    uint32_t rows = dimensions[0];
    uint32_t cols = dimensions[1];
    char** rowPtr = (char**) malloc(rows*sizeof(char*));
    if (rowPtr == NULL){
        printf("Memory could not be allocated for rows of crossword\n");
        abort();
    }
    for (uint32_t i = 0; i < rows; i++){
        rowPtr[i] = (char*) malloc(cols*sizeof(char));
        if (rowPtr[i] == NULL){
            printf("Memory could not be allocated for column %d and beyond\n", i);
            abort();
        }
    }
    return rowPtr;
}

/*
 * this function frees all memory that was allocated on heap by malloc in allocateCrossword() function
 */
void freeCrossword(char** rowPtr, uint32_t dimensions[]){
    uint32_t rows = dimensions[0];
    for (uint32_t i = 0; i < rows; i++){
        free(rowPtr[i]);
        rowPtr[i] = NULL;
        }
    free(rowPtr);
    rowPtr = NULL;
    }

/*
 * this function copies the rows of crossword from text file to dynamically allocated matrix
 */
void copyCrossword(FILE* fptr, uint32_t dimensions[], char** rowPtr){
    uint32_t rows = dimensions[0];
    for (uint32_t i = 0; i < rows; i++){
        fscanf(fptr, "%s", rowPtr[i]);
    }
}

/*
 * This function returns the length of the longest vertical word
 */
uint32_t getMaxLength(word** wordlist){
    uint32_t maxLength = 0;
    for (uint32_t i = 0; i < numDownWords; i++){
        if((wordlist[i]->length) > maxLength){
            maxLength = wordlist[i]->length;
        }
    }
    return maxLength;
}

/*
 * This function prints the longest vertical word(s),each in a new line
 */
void printLongestWords(uint32_t maxLength, word** wordlist){
    printf("The following are the longest vertical words\n");
    for (uint32_t i = 0; i < numDownWords; i++){
        if ((wordlist[i]->length) == maxLength){
            printf("%s\n", (wordlist[i]->word));
        }
    }
}

/*
 * prints position (row and column of first letter) and orientation of word entered by user
 */
void printPosition(word** wordlist, char word[]){
    uint32_t i = 0;
    uint32_t check = 0;
    for (i = 0; i < numWords; i++){
        if (strcasecmp(wordlist[i]->word, word) == 0) {
            printf("row: %d, column: %d, %s\n", wordlist[i]->row, wordlist[i]->col, wordlist[i]->orientation);
            check = 1;
        }
    }
    if ((i == numWords) && (check == 0)){
        printf("Word not found\n");
    }

}

/*
 * prints position (row and column) of each black square which is vertically asymmetric
 * prints "all black squares are vertically symmetric" if there is vertical symmetry
 */
uint32_t isVerticallySymmetric(uint32_t dimensions[], char** rowPtr){
    printf("The following black squares are vertically asymmetric\n");
    uint32_t rows = dimensions[0];
    uint32_t cols = dimensions[1];
    uint32_t leftCounter = 0;
    uint32_t rightCounter = 0;
    for (uint32_t i = 0; i < rows; i++){
        for (uint32_t j = 0; j < cols; j++){
            if (rowPtr[i][j] == '*'){
                leftCounter++;
                if(rowPtr[i][cols-1-j] == '*'){
                    rightCounter++;
                } else {
                    printf("Row: %d, Column: %d\n", i, j);
                }
            }
        }
    }
    if (rightCounter == leftCounter){
        printf("All black squares are vertically symmetric\n");
        return 1;
    } else {
        return 0;
    }
}

uint32_t isHorizontallySymmetric(uint32_t dimensions[], char** rowPtr){
    printf("The following black squares are horizontally asymmetric\n");
    uint32_t rows = dimensions[0];
    uint32_t cols = dimensions[1];
    uint32_t upCounter = 0;
    uint32_t downCounter = 0;
    for (uint32_t j = 0; j < cols; j++){
        for (uint32_t i = 0; i < rows; i++){
            if (rowPtr[i][j] == '*'){
                upCounter++;
                if(rowPtr[rows - 1 - i][j] == '*'){
                    downCounter++;
                } else {
                    printf("Row: %d, Column: %d\n", i, j);
                }
            }
        }
    }
    if (upCounter == downCounter){
        printf("All black squares are horizontally symmetric\n");
        return 1;
    } else {
        return 0;
    }
}

/*
 * checks if crossword has both vertical and horizontal symmetry
 */
void isSymmetric(uint32_t dimensions[], char** rowPtr){
    uint32_t horizontal = isHorizontallySymmetric(dimensions, rowPtr);
    uint32_t vertical = isVerticallySymmetric(dimensions, rowPtr);
    if (horizontal && vertical){
        printf("All black squares are symmetric\n");
    }
}


void main() {
    //Ask user for name of file
   /* char filename[100];
    printf("Enter name of file: ");
    scanf("%s",filename);*/
    FILE* fptr = fopen("crossword1.txt", "r");

    if (fptr == NULL){
         printf("cannot open file\n");
         exit(0);
    }
    uint32_t dimensions[2];

    getDimensions(fptr, dimensions);

    char** rowPtr = allocateCrossword(dimensions);
    copyCrossword(fptr, dimensions, rowPtr);

    word** wordlist = createWordList(dimensions);
    getDownWords(dimensions, rowPtr, wordlist);
    getAcrossWords(dimensions, rowPtr, wordlist);
    uint32_t maxLength = getMaxLength(wordlist);
    printLongestWords(maxLength, wordlist);

    char temp[100];
    printf("Enter a word to search for: ");
    scanf("%s", temp);
    printPosition(wordlist, temp);
    isSymmetric(dimensions, rowPtr);
    freeWordlist(wordlist);
    freeCrossword(rowPtr, dimensions);
    fclose(fptr);

}

