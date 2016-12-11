//
//  FileManager.h
//  SMP2
//
//  Created by Shiuan Lin on 11/29/15.
//

#ifndef FileManager_h
#define FileManager_h

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "HashTable.h"
#include "VarDefine.h"
#include "OnlineCodes.h"

void locationInit(int refWeight[TABLE_SIZE][TABLE_SIZE], LOCATION refTable[]);
// Initializes tables and assigns zeros.
bool locationLoad(int refWeight[TABLE_SIZE][TABLE_SIZE], LOCATION refTable[],char *file[]);
// Loads the data from .txt file and adds the data to the tables.
void locationAdd(int refWeight[TABLE_SIZE][TABLE_SIZE], LOCATION refTable[], char* string);
// Hashes the data and assigns to corresponding cells.
void locationPrint(int refWeight[TABLE_SIZE][TABLE_SIZE]);
// Prints all the edges in the table for debugging.
void locationPrintHash(LOCATION refTable[]);
// Prints all the unique locations in the table;
int  locationHash(LOCATION refTable[], char* key);
// Hashes the key and reslove any collision by calling hash_OpenAddressing().

#endif /* FileManager_h */
