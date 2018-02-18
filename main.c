#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

uint32_t numWords = 0;
uint32_t numDownWords = 0;


typedef struct word{
    uint32_t row;
    uint32_t col;
    uint32_t length;
    char* word;
    char* orientation;
} word;

word** createWordList(uint32_t dimensions[]){
    uint32_t rows = dimensions[0];
    uint32_t cols = dimensions[1];
    word** wordlist = (word**) malloc((rows*cols) * sizeof(word*));

    return wordlist;
}

void buildWord(uint32_t row, uint32_t col, uint32_t length, char* orientation, char* tempword, word** wordlist){
    wordlist[numWords] = (word*) malloc(2*sizeof(char*) + 3 * sizeof(uint32_t));
    wordlist[numWords]->row = row; //+1 here because i has not been incremented yet
    wordlist[numWords]->col = col;
    wordlist[numWords]->length = length;
    wordlist[numWords]->word = (char*) malloc(length*sizeof(char));
    wordlist[numWords]->word = strcpy(wordlist[numWords]->word, tempword);
    wordlist[numWords]->orientation = (char*) malloc(7*sizeof(char));
    strcpy(wordlist[numWords]->orientation, orientation);
}
//checking
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

void getDimensions(FILE* fptr, uint32_t dimensions[]) {
    for (uint32_t i = 0; i < 2; i++) {
        fscanf(fptr, "%d ", &dimensions[i]);
    }
}

char** allocateCrossword(uint32_t dimensions[]) {
    uint32_t rows = dimensions[0];
    uint32_t cols = dimensions[1];
    char** rowPtr = (char**) malloc(rows*sizeof(char*));
    for (uint32_t i = 0; i < rows; i++){
        rowPtr[i] = (char*) malloc(cols*sizeof(char));
    }
    return rowPtr;
}

void freeCrossword(char** rowPtr, uint32_t dimensions[]){
    uint32_t rows = dimensions[0];
    //uint32_t cols = dimensions[1];
    for (uint32_t i = 0; i < rows; i++){
        free(rowPtr[i]);
        rowPtr[i] = NULL;
    }
    free(rowPtr);
    rowPtr = NULL;
}

void copyCrossword(FILE* fptr, uint32_t dimensions[], char** rowPtr){
    uint32_t rows = dimensions[0];
    uint32_t cols = dimensions[1];
    for (uint32_t i = 0; i < rows; i++){
        fscanf(fptr, "%s", rowPtr[i]);
        //printf("%s\n", rowPtr[i]);
    }
}

uint32_t getMaxLength(word** wordlist){
    uint32_t maxLength = 0;
    for (uint32_t i = 0; i < numDownWords; i++){
        if((wordlist[i]->length) > maxLength){
            maxLength = wordlist[i]->length;
        }
    }
    return maxLength;
}

void printLongestWords(uint32_t maxLength, word** wordlist){
    for (uint32_t i = 0; i < numDownWords; i++){
        //printf("%s\n", (wordlist[i]->word));
        if ((wordlist[i]->length) == maxLength){
            printf("%s\n", (wordlist[i]->word));
        }
    }
}

void printPosition(word** wordlist){
    char temp[100];
    printf("Enter a word to search for: ");
    scanf("%s", temp);
    uint32_t i = 0;
    uint32_t check = 0;
    for (i = 0; i < numWords; i++){
        if (strcasecmp(wordlist[i]->word, temp) == 0) {
            printf("row: %d, column: %d, %s\n", wordlist[i]->row, wordlist[i]->col, wordlist[i]->orientation);
            check = 1;
        }
    }
    if ((i == numWords) && (check == 0)){
        printf("Word not found\n");
    }

}

void isSymmetric(uint32_t dimensions[], char** rowPtr){
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
        printf("All black squares are symmetric");
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
    printPosition(wordlist);
    isSymmetric(dimensions, rowPtr);
    freeWordlist(wordlist);
    freeCrossword(rowPtr, dimensions);
    fclose(fptr);

}

