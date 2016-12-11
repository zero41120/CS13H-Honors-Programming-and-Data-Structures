//
//  main.c
//  SMP2
//
//  Created by Shiuan Lin on 11/29/15.
//

#include <stdio.h>
#include "FileManager.h"
#include "VarDefine.h"
#include "Graph.h"

int main(int argc, char * argv[]) {
    
    int myLocationWeight[TABLE_SIZE][TABLE_SIZE];
    LOCATION myLocationTable[TABLE_SIZE];
    char buffer[BUFFER_STRING_SIZE];
    arrayInit(buffer, BUFFER_STRING_SIZE);
    if(locationLoad(myLocationWeight, myLocationTable, argv) == false){
        return 0;
    }

    ROUTE *myRouteHead = (ROUTE*)malloc(sizeof(ROUTE*));
    myRouteHead->name = (char*)malloc(sizeof(char*));
    myRouteHead->nextLocation = NULL;
    
    ROUTE *myBestRoute = (ROUTE*)malloc(sizeof(ROUTE*));
    myBestRoute->name = (char*)malloc(sizeof(char*));
    myBestRoute->nextLocation = NULL;
    
    //locationPrint(myLocationWeight);
    if(getLocationFromUser(myLocationTable, buffer) == false){
        return 0; //END
    }
    char *homeKey = strdup(buffer);
    int totalDistance = 0, bestDistance = 9999999, DFSCount = 0;
    depthFirstSearch(myLocationWeight, myLocationTable, myRouteHead, myBestRoute, buffer, homeKey, &totalDistance, &bestDistance, &DFSCount);

    if (strcmp(myBestRoute->name, "\0") == 0) {
        puts("There is no Hamiltonian Path, I am sorry");
    } else {
        printf("The best route is %d miles! Follow this path: \n", bestDistance);
        printLinkLocation(myBestRoute);
    }
    
    return 0; //END
}
