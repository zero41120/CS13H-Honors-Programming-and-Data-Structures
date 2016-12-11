//
//  OnlineCodes.c
//  Graph
//
//  Created by Shiuan Lin on 11/29/15.
//

#include "OnlineCodes.h"

char *trimwhitespace(char *str) {
    char *end;
    while(isspace(*str)) str++;
    
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;
    *(end+1) = 0;
    return str;
}