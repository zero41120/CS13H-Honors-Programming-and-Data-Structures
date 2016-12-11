//
//  speak.c
//  HashTable
//
//  Created by Shiuan Lin on 11/11/15.
//

#include "speak.h"
#define BUFFER_TEXT_BUILDER_SIZE 5000

char* strdup(const char *s);
// Override, 
bool speakLoad(char *argv[], SPEAK *mySpeak[], BLOOM *myFilter){
    FILE *filePointer;
    char buffer[BUFFER_WORD_SIZE];
    if(!(filePointer=fopen(argv[2], "r"))) {
        fprintf(stderr, "ERROR: Could not open file %s\n", argv[2]);
        return false;
    }
    while (fgets(buffer, BUFFER_WORD_SIZE, filePointer) != NULL) speakAdd(mySpeak, myFilter, buffer);
    fclose(filePointer);
    return true;
    
} // Takes the second command line argument. Please pass in the Old : New dictionary.

void speakInit(SPEAK *mySpeak[]){
    int i = 0;
    for (i = 0; i < SPEAK_TABLE_SIZE; i++) {
        mySpeak[i] = malloc(sizeof(SPEAK));
        mySpeak[i]->oldSpeak = (char*)malloc(sizeof(char*));
        mySpeak[i]->newSpeak = (char*)malloc(sizeof(char*));
        mySpeak[i]->nextSpeak = NULL;
    }
} // Allocates speace for the table and the data it holds.


void speakAdd(SPEAK *mySpeak[], BLOOM *myFilter, char *word_Divided_By_Colon){
    char *tempNew, *tempOld = strdup(word_Divided_By_Colon);
    unsigned int index, i;
    tempOld = strtok(tempOld, " :"); // Select old speech
    tempNew = strtok(NULL, " :\n");  // Select new speech
    for (i = 0; tempOld[i] != '\0'; i++) tempOld[i] = tolower(tempOld[i]);
    for (i = 0; tempNew[i] != '\0'; i++) tempNew[i] = tolower(tempNew[i]);
    bloomAdd(myFilter, tempNew); // Add new speech into the filter
    index = speakHash(tempOld);
    speakAdd_r(mySpeak[index], tempOld, tempNew);
} // Adds old and new speech in the speak table. Add new speech into the bloom table.

void speakAdd_r(SPEAK *refSpeak,char *tempOld, char *tempNew){
    if (strcmp(refSpeak->oldSpeak, "") != 0) {
        while (refSpeak->nextSpeak != NULL) refSpeak = refSpeak->nextSpeak;
        // If the cell is a linked list, change the pointer to nextSpeak;
        refSpeak->nextSpeak = malloc(sizeof(SPEAK));
        refSpeak->nextSpeak->oldSpeak = (char*)malloc(sizeof(char*));
        refSpeak->nextSpeak->newSpeak = (char*)malloc(sizeof(char*));
        refSpeak->nextSpeak->nextSpeak = NULL;
        speakAdd_r(refSpeak->nextSpeak, tempOld, tempNew);
    } else {
        refSpeak->oldSpeak = strdup(tempOld);
        refSpeak->newSpeak = strdup(tempNew);
    }
} // Adds old and new speech in the nextSpeak of the table. (Linked list)

bool speakCheck(SPEAK *mySpeak[], char *singleWord){
    unsigned int index = speakHash(singleWord);
    if (strcmp(mySpeak[index]->oldSpeak, "") == 0) return false;
    return speakCheck_r(mySpeak[index], singleWord);
} // Checks if the text has old speech.

bool speakCheck_r(SPEAK *refSpeak, char *singleWord){
    if (strcmp(refSpeak->oldSpeak, singleWord) != 0) {
        if (refSpeak->nextSpeak != NULL) {
            refSpeak = refSpeak->nextSpeak;
            return speakCheck_r(refSpeak, singleWord);
        } else {
            return false;
        }
    }
    return true;
} // Checks if the text had old speech in the nextSpeak of the table. (Linked list)

void speakPrintNewText(SPEAK *mySpeak[], char *text){
    char buffer[BUFFER_TEXT_BUILDER_SIZE];
    int i;
    for (i = 0; i <BUFFER_TEXT_BUILDER_SIZE; i++) buffer[i] = '\0';
    for (i = 0; text[i] != '\0'; i++) text[i] = tolower(text[i]);
    if (i>=BUFFER_TEXT_BUILDER_SIZE) {
        puts("ERROR:Text size overflow");
        return;
    }

    char *singleWord = strdup(text);
    singleWord = strtok(singleWord, " \t,.;:\r\n?!-/()@#$%^&*_+1234567890");
    while (singleWord != NULL) {
        strcat(buffer, speakPrintNewTextBuilder(mySpeak, singleWord));
        strcat(buffer, " ");
        singleWord = strtok(NULL, " \t,.;:\r\n?!-/()@#$%^&*_+1234567890");
    }
    
    if (strcmp(buffer, "") == 0) {
        printf("(Perfect)\n");
    } else {
        printf("(Fixed)%s(Fixed)\n", buffer);
    }
} // Prints new speech that replace all the ungood speech.


char* speakPrintNewTextBuilder(SPEAK *mySpeak[], char *singleWord){
    int index = speakHash(singleWord);
    if (strcmp(mySpeak[index]->oldSpeak, "") == 0) return singleWord;
    if (strcmp(mySpeak[index]->oldSpeak, singleWord) == 0) return mySpeak[index]->newSpeak;
    if (mySpeak[index]->nextSpeak != NULL) return speakPrintNewTextBuilder_r(mySpeak[index]->nextSpeak, singleWord);
    return singleWord;
} // Returns new speech to build up new text.

char* speakPrintNewTextBuilder_r(SPEAK *refSpeak, char *singleWord){
    if (strcmp(refSpeak->oldSpeak, singleWord) != 0) {
        if (refSpeak->nextSpeak != NULL) {
            refSpeak = refSpeak->nextSpeak;
            return speakPrintNewTextBuilder_r(refSpeak, singleWord);
        } else {
            return singleWord;
        }
    }
    return refSpeak->newSpeak;
} // Returns new speech in nextSpeak to build up new text. (Linked list)

void speakPrintTable(SPEAK *mySpeak[]){
    int i = 0;
    for (i = 0; i < SPEAK_TABLE_SIZE; i++) {
        printf("%d: %s = %s",i ,mySpeak[i]->oldSpeak,mySpeak[i]->newSpeak);
        SPEAK *temp = mySpeak[i]->nextSpeak;
        while (temp != NULL) {
            printf(" ->%s = %s ",temp->oldSpeak,temp->newSpeak);
            temp = temp->nextSpeak;
        }
        puts("\n");
    }
    printf("Speak table size: %d\n", SPEAK_TABLE_SIZE);
} // For debug purpose, shows all the location of the new speeches

unsigned int speakHash(char *key){
    unsigned int i, h_index = 2145633751;
    int someNumber = 13758619;
    for (i = 0; key[i] != '\n' && key[i] != '\0'; i++) {
        h_index = (h_index * someNumber) ^ key[i];
    }
    return h_index % SPEAK_TABLE_SIZE;
} // Index = (Some number) * index + the key.

