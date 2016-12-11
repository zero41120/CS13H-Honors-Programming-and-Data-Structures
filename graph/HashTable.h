//
//  HashTable.h
//  Graph
//
//  Created by Shiuan Lin on 11/24/15.
//

#ifndef HashTable_h
#define HashTable_h

#include <stdio.h>
#include "VarDefine.h"

int hash(char *key);
int hash_OpenAddressing(char *key, unsigned int trail);
int num(int x, int y);
#endif /* HashTable_h */
