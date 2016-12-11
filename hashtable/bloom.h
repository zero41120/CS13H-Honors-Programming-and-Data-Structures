//
//  bloom.h
//  HashTable
//
//  Created by Shiuan Lin on 11/10/15.
//

#ifndef bloom_h
#define bloom_h

#include <stdio.h>
#include <ctype.h>      // tolower();
#include <stdbool.h>    // bool

#define BLOOM_TABLE_SIZE 400009
#define BLOOM_BIT_SIZE (BLOOM_TABLE_SIZE * 8)
#define BUFFER_WORD_SIZE 1000

// Credit to Dr. Long
# define	SETBIT(A, k) A[k >> 3] |=  (01 << (k & 07))
# define	CLRBIT(A, k) A[k >> 3] &= ~(01 << (k & 07))
# define	GETBIT(A, k) (A[k >> 3] & (01 << (k & 07)))

typedef unsigned char BLOOM;

bool bloomLoad(char *argv[], BLOOM *myFilter);
// Takes the first command line argument. Please pass in the English dictionary.
void bloomInit(BLOOM *table);
// Assigns zeros to all the bits.
void bloomAdd(BLOOM *table, char *key);
// Adds the key to the table with hash index. This function calls the bloomHash_x to create the bit index.
bool bloomCheck(BLOOM *table, char *key);
// Checks the key using bloomHash_x against the table.
void bloomPrintTable(BLOOM myFilter[]);
// For debug purpose, shows all bits in hexdecimal.
unsigned int bloomHash_0(char *key);
// Index = [(Some number) * Index] XOR with the key.
unsigned int bloomHash_1(char *key);
// Index = (Some number) * index + the key.

#endif /* bloom_h */
