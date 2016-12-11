//
//  FileManager.c
//  SMP2
//
//  Created by Shiuan Lin on 11/29/15.
//

#include "FileManager.h"

//#define STEP

void locationInit(int refWeight[TABLE_SIZE][TABLE_SIZE], LOCATION refTable[]){
    int inner = 0, outter = 0;
    
    for (outter = 0; outter < TABLE_SIZE; outter++) {
        for (inner = 0; inner < TABLE_SIZE ; inner++) {
            refWeight[outter][inner] = 0;
        }
    }
    
    for (inner = 0; inner <TABLE_SIZE; inner++) {
        refTable[inner].name = malloc(sizeof(char*));
        refTable[inner].isVisited = false;
    }
} // Initializes tables and assigns zeros.

bool locationLoad(int refWeight[TABLE_SIZE][TABLE_SIZE], LOCATION refTable[],char *file[]){
    FILE *filePointer;
    char buffer[BUFFER_STRING_SIZE];
    
    if(!(filePointer=fopen(file[1], "r"))) {
        fprintf(stderr, "ERROR: Could not open file %s\n", file[1]);
        return false;
    }
    
    locationInit(refWeight, refTable);
    
    while (fgets(buffer, BUFFER_STRING_SIZE, filePointer) != NULL){
        locationAdd(refWeight, refTable ,buffer);
    }
    
    fclose(filePointer);
    return true;
} // Loads the data from .txt file and adds the data to the tables.

void locationAdd(int refWeight[TABLE_SIZE][TABLE_SIZE], LOCATION refTable[], char* string){
    char *from = strdup(string), *to;
    int  distance, index1, index2;
    
    from     = strtok(from, ":");
    to       = strtok(NULL, ":");
    distance = (int)atoi(strtok(NULL, " :\n"));
    
    from = trimwhitespace(from);
    to   = trimwhitespace(to);
    
    index1 = locationHash(refTable, from);
    index2 = locationHash(refTable, to);
    
    refWeight[index1][index2] = distance;
    
#ifdef STEP
    printf("-[%02d][%02d] = %02d | %s -> %s\n", index1, index2, distance, from, to);
#endif // STEP
} // Hashes the data and assigns to corresponding cells.


int locationHash(LOCATION refTable[], char* key){
    int index = hash(key), trail = 1;
#ifdef STEP
    printf("Key: [%02d]%s, %02dH: [%02d]%s\n", index, key, trail , index, refTable[index].name);
#endif // STEP
    if(strcmp(refTable[index].name, key) == 0){
        return index;
    }
    if (strcmp(refTable[index].name, "\0") == 0){
        refTable[index].name = strdup(key);
        return index;
    }
    index = hash_OpenAddressing(key, trail++);
#ifdef STEP
    printf("Key: [%02d]%s, %02dH: [%02d]%s\n", index, key, trail , index, refTable[index].name);
#endif // STEP
    while (strcmp(refTable[index].name, key) != 0) {
        if (strcmp(refTable[index].name, "\0") == 0) {
            refTable[index].name = strdup(key);
            return index;
        }
        index = hash_OpenAddressing(key, trail++);
#ifdef STEP
    printf("Key: [%02d]%s, %02dH: [%02d]%s\n", index, key, trail , index, refTable[index].name);
#endif // STEP
    }
    return index;
} // Hashes the key and reslove any collision by calling hash_OpenAddressing().


void locationPrint(int refWeight[TABLE_SIZE][TABLE_SIZE]){
    int inner = 0, outter = 0;
    for (outter = 0; outter < TABLE_SIZE; outter++) {
        for (inner = 0; inner < TABLE_SIZE ; inner++) {
            if(refWeight[outter][inner] > 0){
                printf("[%02d][%02d] = %02d\n", outter, inner, refWeight[outter][inner]);
            }
        }
    }
} // Prints all the edges in the table for debugging.

void locationPrintHash(LOCATION refTable[]){
    int inner = 0;
    for (inner = 0; inner < TABLE_SIZE ; inner++) {
        if (strcmp(refTable[inner].name, "\0") != 0) {
            printf("[%02d] = %s\n", inner, refTable[inner].name);
        }
    }
} // Prints all the unique locations in the table;






