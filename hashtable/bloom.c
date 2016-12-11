//
//  bloom.c
//  HashTable
//
//  Created by Shiuan Lin on 11/10/15.
//

#include "bloom.h"

bool bloomLoad(char *argv[], BLOOM *myFilter){
    FILE *filePointer;
    char buffer[BUFFER_WORD_SIZE];
    if(!(filePointer=fopen(argv[1], "r"))) {
        fprintf(stderr, "ERROR: Could not open file %s\n", argv[1]);
        return false;
    }
    while (fgets(buffer, BUFFER_WORD_SIZE, filePointer) != NULL) bloomAdd(myFilter, buffer);
    fclose(filePointer);
    return true;
} // Takes the first command line argument. Please pass in the English dictionary.

void bloomInit(BLOOM *table){
    int i = 0;
    for (i = 0; i <BLOOM_TABLE_SIZE; i++) table[i] = 0;
} // Assigns zeros to all the bits.

void bloomAdd(BLOOM *table, char *key){
    int i = 0;
    for (i = 0; key[i] != '\0'; i++) tolower(key[i]);
    SETBIT(table, bloomHash_0(key));
    SETBIT(table, bloomHash_1(key));
} // Adds the key to the table with hash index. This function calls the bloomHash_x to create the bit index.

bool bloomCheck(BLOOM *table, char *key){
    int i = 0;
    for (i = 0; key[i] != '\0'; i++) tolower(key[i]);
    if(GETBIT(table, bloomHash_0(key)) && GETBIT(table, bloomHash_1(key))) return true;
    return false;
} // Checks the key using bloomHash_x against the table.

void bloomPrintTable(BLOOM myFilter[]){
    int i = 0;
    for (i = 0; i < BLOOM_TABLE_SIZE; i++) printf("%x",myFilter[i]);
    printf("\nBloom bit size: %d\n", BLOOM_BIT_SIZE);
    printf("Bloom table size: %d\n", BLOOM_TABLE_SIZE);
} // For debug purpose, shows all bits in hexdecimal.

unsigned int bloomHash_0(char *key){
    unsigned int i, h_index = 2145633751;
    int someNumber = 13758619;
    for (i = 0; key[i] != '\n' && key[i] != '\0'; i++) {
        h_index = (unsigned int)(h_index * someNumber) ^ key[i];
    }
    return h_index % BLOOM_BIT_SIZE;
} // Index = [(Some number) * Index] XOR with the key.

unsigned int bloomHash_1(char *key){
    unsigned int i, h_index = 0;
    double someNumber = 31.61803398875;
    for (i = 0; key[i] != '\n' && key[i] != '\0'; i++) {
        h_index = (unsigned int)(someNumber * h_index + (unsigned int)key[i]);
    }
    return h_index % BLOOM_BIT_SIZE;
} // Index = (Some number) * index + the key.

