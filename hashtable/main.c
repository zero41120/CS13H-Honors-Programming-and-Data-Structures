//
//  main.c
//  HashTable
//
//  Created by Shiuan Lin on 11/10/15.
//

#include <stdio.h>
#include <stdbool.h>    // bool
#include <string.h>     // strdup(); strcmp();
#include <ctype.h>      // tolower();

#include "bloom.h"
#include "speak.h"

//#define DEBUG_MODE
#define BUFFER_WORD_SIZE 1000

char* strdup(const char *s);
// Override the return type. centOS compiler needs this.
void fileInit(BLOOM *myFilter, SPEAK *mySpeak[], char *argv[]);
// Takes command line arguments and assigns them to the hashtables.
// First argument must be English dictionary.
// Second argument must be Old : New speech dictionary.
// Example: ./SantaCruzFirewall words.txt nadsat.txt
// If you want to have txt file with testing speech, do this
// ./SantaCruzFirewall words.txt nadsat.txt < testInput.txt

bool checkEnglishText(char *text, int *thoughtCrimeCount, BLOOM *myFilter);
// Checks words that's not in English
void checkOldSpeak(char *text, SPEAK *mySpeak[]);
// Checks old speech and print out new speech.

// Global Variable
bool running = true;
// Global Variable

int main(int argc, char *argv[]){
    BLOOM myFilter[BLOOM_TABLE_SIZE];
    SPEAK *mySpeak[SPEAK_TABLE_SIZE];
    char  buffer[BUFFER_WORD_SIZE];
    int   thoughtCrimeCount = 0;
    
    fileInit(myFilter, mySpeak, argv);
    
    puts("Enter text:");
    while (running && fgets(buffer, BUFFER_WORD_SIZE, stdin)) {
        printf("%s", buffer);
        if(checkEnglishText(buffer, &thoughtCrimeCount, myFilter)) {
            checkOldSpeak(buffer, mySpeak);
            // Check speech if all words are English.
        }
        running == true? 0:puts("\nYou really enjoy the joycamp!\n\n");
    }
}

void fileInit(BLOOM *myFilter, SPEAK *mySpeak[], char *argv[]){
    bloomInit(myFilter);
    speakInit(mySpeak);
    bloomLoad(argv, myFilter);
    speakLoad(argv, mySpeak, myFilter);
#ifdef DEBUG_MODE
    bloomPrintTable(myFilter);
    speakPrintTable(mySpeak);
#endif // DEBUG_MODE
} // Takes command line arguments and assign to the hashtables.

bool checkEnglishText(char *text, int *thoughtCrimeCount, BLOOM *myFilter){
    int i = 0;
    for (i = 0; text[i] != '\0'; i++) text[i] = tolower(text[i]);
    
    char *singleWord = strdup(text);
    singleWord = strtok(singleWord, " \t,.;:\r\n?!-/()@#$%^&*_+1234567890");
    
    while (singleWord != NULL) {
        if (!bloomCheck(myFilter, singleWord)) {
            switch (*thoughtCrimeCount) {
                case 0:
                    printf(" >%s< Doubleplus ungood word! 1/3\n", singleWord);
                    *thoughtCrimeCount = *thoughtCrimeCount + 1;
                    return false;
                    break;
                case 1:
                    printf(" >%s< Doubleplus ungood word! 2/3\n", singleWord);
                    *thoughtCrimeCount = *thoughtCrimeCount + 1;
                    return false;
                    break;
                case 2:
                    printf(" >%s< You are now transfering to the joycamp! 3/3\n\n",singleWord);
                    running = false;
                    return false;
                    break;
            } // switch
        } // if
        singleWord = strtok(NULL, " \t,.;:\r\n?!-/()@#$%^&*_+1234567890");
    } // while
#ifdef DEBUG_MODE
    printf("Bloom check passed.\n\n");
#endif // DEBUG_MODE
    return true;
} // Checks words that's not in English

void checkOldSpeak(char *text, SPEAK *mySpeak[]){
    int i = 0;
    for (i = 0; text[i] != '\0'; i++) text[i] = tolower(text[i]);
    
    char *singleWord = strdup(text);
    singleWord = strtok(singleWord, " \t,.;:\r\n?!-/()@#$%^&*_+1234567890");
    
    while (singleWord != NULL) {
        if (speakCheck(mySpeak, singleWord)){
            speakPrintNewText(mySpeak, text);
            return;
        }
        singleWord = strtok(NULL, " \t,.;:\r\n?!-/()@#$%^&*_+1234567890");
    }
    printf("(Perfect)\n");
} // Checks old speech and print out new speech.



