//
//  speak.h
//  HashTable
//
//  Created by Shiuan Lin on 11/11/15.
//

#ifndef speak_h
#define speak_h

#include <stdio.h>
#include <stdbool.h>    // bool
#include <string.h>     // strdup(); strcmp();
#include <stdlib.h>     // malloc();
#include <ctype.h>      // tolower();
#include "bloom.h"      // hash new speech into the filter

#define SPEAK_TABLE_SIZE 307

typedef struct speakData SPEAK;
struct speakData{
    char    *oldSpeak;
    char    *newSpeak;
    SPEAK   *nextSpeak;
};

bool speakLoad(char *argv[], SPEAK *mySpeak[], BLOOM *myFilter);
// Takes the second command line argument. Please pass in the Old : New dictionary.
void speakInit(SPEAK *mySpeak[]);
// Allocates speace for the table and the data it holds.
void speakAdd(SPEAK *mySpeak[], BLOOM *myFilter, char *word_Divided_By_Colon);
// Adds old and new speech in the speak table. Add new speech into the bloom table.
void speakAdd_r(SPEAK *refSpeak,char *tempOld, char *tempNew);
// Adds old and new speech in the nextSpeak of the table. (Linked list)
bool speakCheck(SPEAK *mySpeak[], char *singleWord);
// Checks if the text has old speech.
bool speakCheck_r(SPEAK *refSpeak, char *singleWord);
// Checks if the text had old speech in the nextSpeak of the table. (Linked list)
void speakPrintNewText(SPEAK *mySpeak[], char *text);
// Prints new speech that replace all the ungood speech.
char* speakPrintNewTextBuilder(SPEAK *mySpeak[], char *singleWord);
// Returns new speech to build up new text.
char* speakPrintNewTextBuilder_r(SPEAK *refSpeak, char *singleWord);
// Returns new speech in nextSpeak to build up new text. (Linked list)
void speakPrintTable(SPEAK *mySpeak[]);
// For debug purpose, shows all the location of the new speeches
unsigned int speakHash(char *key);
// Index = (Some number) * index + the key.

#endif /* speak_h */
