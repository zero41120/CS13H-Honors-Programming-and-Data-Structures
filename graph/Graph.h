//
//  Graph.h
//  SMP2
//
//  Created by Shiuan Lin on 11/29/15.
//

#ifndef Graph_h
#define Graph_h

#include <stdio.h>
#include "VarDefine.h"
#include "HashTable.h"
#include "FileManager.h"

void depthFirstSearch(int refWeight[TABLE_SIZE][TABLE_SIZE], LOCATION refTable[], ROUTE *refRoute, ROUTE *bestRoute, char* key, char* homeKey, int *totalDistance, int *bestDistance, int *DFSCount);
// The depth first search. It does all the jobs.
bool getLocationFromUser(LOCATION refTable[], char* buffer);
// Reads input from termianl.
bool isExisted(LOCATION refTable[], char* key);
// Checks the data according to the key.
bool isAllVisited(LOCATION refTable[]);
// Checks if visited all locations.
int  getIndex(LOCATION refTable[], char* key);
// Hashes the key and resloves any collision.
void linkLocation(ROUTE *refRoute, char *key);
// Links location to route linked list.
void unlinkLocation(ROUTE *refRoute);
// Unlinks location from the linked list.
void printLinkLocation(ROUTE *refRoute);
// Prints the route linked list.
void storeBestRoute(ROUTE *refRoute, ROUTE *bestRoute);
// If the program finds a better route, replaces old one.

#endif /* Graph_h */
