//
//  HashTable.c
//  Graph
//
//  Created by Shiuan Lin on 11/24/15.
//

#include "HashTable.h"

int hash(char *key){
    unsigned int i, h_index = 2145633751;
    int someNumber = 13758619;
    for (i = 0; key[i] != '\n' && key[i] != '\0'; i++) {
        h_index = (unsigned int)(h_index * someNumber) ^ key[i];
    }
    return h_index % TABLE_SIZE;
}

int hash_OpenAddressing(char *key, unsigned int trail){
    unsigned int previous = hash(key);
    return (previous + (unsigned int)num(2, trail)) % TABLE_SIZE;
}

int num(int x, int y){
    int i;
    for (i = 0; i < y; i++) {
        x *=x;
    }
    return x;
}