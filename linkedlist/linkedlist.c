//
//  linkedlist.c
//  LinkedList
//
//  Created by Shiuan Lin on 10/12/15.
//

#include <stdio.h>
#include <stdbool.h>    // bool
#include <stdlib.h>     // free()
#include <string.h>     // strdup()
#include <ctype.h>      // tolower()

//#define DEBUGMODE
#define MAX_CHAR_SIZE 50

typedef struct linkedWord Word;
struct linkedWord{
    char    *data;
    int     count;
    Word    *nextWord;
};


// Read input from stdin
char* getWord();
// Add the input to both lists
void initWithData(Word *refWordBasic, Word *refWordMTFR,char* refChar);
// Print out both list. |Frequency-Word
void printList();
// Insert the word. If the word is already in the list, free the word.
int insertBasic(Word *refWord, char* refString);
int insertMTFR(Word *refWord, char* refString);


// Global var.
Word *BasicHead = NULL;
Word *MTFRHead  = NULL;
bool running    = true;
int compareInBasic = 0, compareInMTFR = 0;




int main(int argc, const char * argv[]) {

    printf("Eric's linked list.\nPut your texts here:");

    Word *myBasicList   = (Word*)malloc(sizeof(Word));
    Word *myMTFRList    = (Word*)malloc(sizeof(Word));
    BasicHead           = myBasicList;
    MTFRHead            = myMTFRList;
    initWithData(myBasicList, myMTFRList, getWord());
    
    printf("Put your texts again:");

    while (running) {
        
        Word* createBasic   = (Word*)malloc(sizeof(Word));
        Word* createMTFR    = (Word*)malloc(sizeof(Word));
        initWithData(createBasic, createMTFR, getWord());
        
        compareInBasic  = compareInBasic + insertBasic(createBasic, createBasic->data);
        compareInMTFR   = compareInMTFR  + insertMTFR(createMTFR, createMTFR->data);
        
        printf("Compare# in basic: %d\n", compareInBasic);
        printList(myBasicList);
        printf("Compare# in MTFR : %d\n", compareInMTFR);


    } // End while
} // End Main

void initWithData(Word *refWordBasic, Word *refWordMTFR, char* refChar){
    
    refWordBasic->data = strdup(refChar);
    refWordBasic->count = 1;
    refWordBasic->nextWord = NULL;
    
    refWordMTFR->data = strdup(refChar);
    refWordMTFR->count = 1;
    refWordMTFR->nextWord = NULL;
}





int insertBasic(Word* refWord, char* refString){
    
    Word *myList = BasicHead;
    int compareCounter = 1;
    
    // Check the first word for repeat.
    if (strcmp(myList->data, refString)== 0) {
        myList->count++;
        free(refWord);
        return compareCounter;
    } // End if
    
    // Check all other words after first word for repeat.
    while (myList->nextWord != NULL){
        myList = myList->nextWord;
        compareCounter++;
        if (strcmp(myList->data, refString)== 0) {
            myList->count++;
            free(refWord);
            return compareCounter;
        } // End if
    } // End while
    
    // No repeat, link the refWord to the list.
    myList->nextWord = refWord;
    return compareCounter;
}

int insertMTFR(Word *refWord, char* refString){
    
    Word *myList = MTFRHead;
    Word *prevTemp = NULL;
    int compareCounter = 1;
    
    // Check the first word for repeat.
    if (strcmp(myList->data, refString)== 0) {
        myList->count++;
        free(refWord);
        return compareCounter;
    } // End if
    
    // Check all other words after first word for repeat.
    while (myList->nextWord != NULL){
        prevTemp = myList;
        myList = myList->nextWord;
        compareCounter++;
        if (strcmp(myList->data, refString)== 0) {
            myList->count++;
            // Swap the link pointer.
            prevTemp->nextWord = myList->nextWord;
            myList->nextWord = MTFRHead;
            MTFRHead = myList;
            free(refWord);
            return compareCounter;
        }
    } // End while
    
    // No repeat, link the refWord to the list.
    myList->nextWord = refWord;
    return compareCounter;
}


char* getWord(){
    
    static char buffer[MAX_CHAR_SIZE];
    // If fgets reads End of File, stop running the program.
    if(fgets (buffer , MAX_CHAR_SIZE , stdin) == NULL){
        running = false;
    }
    // Make words in lower case, and replace any punctuation with space.
    int i = 0;
    for (i = 0; buffer[i] != '\0'; i++) {
        buffer[i] = tolower(buffer[i]);
        if (buffer[i] <97 || buffer[i] >122){
            buffer[i] = ' ';
        }
    }
    return buffer;
}

void printList(){
    
    Word *tempBasicList = BasicHead;
    Word *tempMTFRList  = MTFRHead;

    printf("Basic:");
    while (tempBasicList->nextWord != NULL){
        printf("|%d-%s", tempBasicList->count, tempBasicList->data);
        tempBasicList  = tempBasicList->nextWord;
    }
    printf("|%d-%s\n", tempBasicList->count, tempBasicList->data);
    
    
    printf("MTFR :");
    while (tempMTFRList->nextWord != NULL){
        printf("|%d-%s", tempMTFRList->count, tempMTFRList->data);
        tempMTFRList   = tempMTFRList->nextWord;
    }
    printf("|%d-%s\n", tempMTFRList->count, tempMTFRList->data);

    
}
