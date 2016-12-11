//
//  VarDefine.h
//  SMP2
//
//  Created by Shiuan Lin on 11/29/15.
//

#ifndef VarDefine_h
#define VarDefine_h

#include <stdbool.h>

#define TABLE_SIZE 91
#define BUFFER_STRING_SIZE 100

#define arrayInit(array, size) {int i; for (i = 0; i < size; i++) { array[i] = '\0';}}

typedef struct locationData LOCATION;
struct locationData{
    char* name;
    bool isVisited;
};

typedef struct routeList ROUTE;
struct routeList{
    char* name;
    ROUTE *nextLocation;
};

char* strdup(const char* s);


#endif /* VarDefine_h */
