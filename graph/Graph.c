//
//  Graph.c
//  SMP2
//
//  Created by Shiuan Lin on 11/29/15.
//

#include "Graph.h"

void depthFirstSearch(int refWeight[TABLE_SIZE][TABLE_SIZE], LOCATION refTable[], ROUTE *refRoute, ROUTE *bestRoute, char* key, char* homeKey, int *totalDistance, int *bestDistance, int *DFSCount){
    ++*DFSCount;
    int fromIndex = locationHash(refTable, key);
    refTable[fromIndex].isVisited = true;
    linkLocation(refRoute, refTable[fromIndex].name);
    if(isAllVisited(refTable)){
        int homeIndex = getIndex(refTable, homeKey);
        if (refWeight[fromIndex][homeIndex] > 0) {
            *totalDistance += refWeight[fromIndex][homeIndex];
            linkLocation(refRoute, homeKey);
            printf("HAMOTIONIAN! TotalDistance:%d DFSCount:%d\n", *totalDistance, *DFSCount);
            if (*totalDistance < *bestDistance) {
                *bestDistance = *totalDistance;
                storeBestRoute(refRoute, bestRoute);
            }
            *totalDistance -= refWeight[fromIndex][homeIndex];
            printLinkLocation(refRoute);
            unlinkLocation(refRoute);
        }
    }
    int toIndex = 0;
    for (toIndex = 0; toIndex < TABLE_SIZE; toIndex++) {
        if(refWeight[fromIndex][toIndex] > 0){
            if (refTable[toIndex].isVisited == false) {
                *totalDistance += refWeight[fromIndex][toIndex];
                depthFirstSearch(refWeight, refTable, refRoute, bestRoute, refTable[toIndex].name, homeKey, totalDistance, bestDistance, DFSCount);
                unlinkLocation(refRoute);
                *totalDistance -= refWeight[fromIndex][toIndex];
            }
        }
    } // for
    
    refTable[fromIndex].isVisited = false;
    unlinkLocation(refRoute);
}

bool getLocationFromUser(LOCATION refTable[], char *buffer){
    puts("Please enter a city name, you may choose any city here:");
    locationPrintHash(refTable);

    if (fgets(buffer, BUFFER_STRING_SIZE, stdin) == NULL) {
        return false;
    }
    strtok(buffer, "\n");
    
    if(isExisted(refTable, buffer) == false){
        puts("Not vaild input, please enter location again.");
        getLocationFromUser(refTable, buffer);
    }
    
    return true;
}


bool isExisted(LOCATION refTable[], char* key){
    int index = hash(key), trail = 1;

    if(strcmp(refTable[index].name, key) == 0){
        return true;
    }
    if (strcmp(refTable[index].name, "\0") == 0){
        return false;
    }
    index = hash_OpenAddressing(key, trail++);

    while (strcmp(refTable[index].name, key) != 0) {
        if (strcmp(refTable[index].name, "\0") == 0) {
            return false;
        }
        index = hash_OpenAddressing(key, trail++);
    }
    return false;
}

bool isAllVisited(LOCATION refTable[]){
    int i = 0;
    for (i = 0; i < TABLE_SIZE; i++) {
        if (strcmp(refTable[i].name, "\0") != 0) {
            if(refTable[i].isVisited == false){
                return false;
            }
        }
    }
    return true;
}

int getIndex(LOCATION refTable[], char* key){
    int index = hash(key), trail = 1;
    if(strcmp(refTable[index].name, key) == 0) {
        return index;
    }
    
    index = hash_OpenAddressing(key, trail++);
    while (strcmp(refTable[index].name, key) != 0) {
        index = hash_OpenAddressing(key, trail++);
    }
    return index;

}

void linkLocation(ROUTE *refRoute, char *key){
    if (strcmp(refRoute->name, "\0") == 0) {
        refRoute->name = strdup(key);
        refRoute->nextLocation = NULL;
    } else if(refRoute->nextLocation == NULL){
        refRoute->nextLocation = (ROUTE*)malloc(sizeof(ROUTE*));
        refRoute->nextLocation->name = strdup("\0");
        refRoute = refRoute->nextLocation;
        linkLocation(refRoute, key);
    } else {
        refRoute = refRoute->nextLocation;
        linkLocation(refRoute, key);
    }
}


void unlinkLocation(ROUTE *refRoute){
    while (refRoute->nextLocation != NULL) {
        if (strcmp(refRoute->nextLocation->name, "\0") == 0) {
            refRoute->nextLocation = NULL;
            return;
        }
        refRoute = refRoute->nextLocation;
    }
    refRoute->name = strdup("\0");
}

void printLinkLocation(ROUTE *refRoute){
    while (refRoute->nextLocation != NULL) {
        printf("%s -> ",refRoute->name);
        refRoute = refRoute->nextLocation;
    }
    printf("%s",refRoute->name);
    puts("\n");
}

void storeBestRoute(ROUTE *refRoute, ROUTE *bestRoute){
    while (strcmp(refRoute->name, "\0") != 0) {
        bestRoute->name = strdup("\0");
        linkLocation(bestRoute, refRoute->name);
        if ((refRoute->nextLocation == NULL) || (strcmp(refRoute->nextLocation->name, "\0") == 0)) {
            return;
        }
        if (bestRoute->nextLocation == NULL) {
            bestRoute->nextLocation = (ROUTE*)malloc(sizeof(ROUTE*));
            bestRoute->nextLocation->name = strdup("\0");
            bestRoute->nextLocation->nextLocation = NULL;
        }
        bestRoute = bestRoute->nextLocation;
        refRoute = refRoute->nextLocation;
    }
}
