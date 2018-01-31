#ifndef GEDCOMUTIL_H
#define GEDCOMUTIL_H

#include <ctype.h>
#include "GEDCOMparser.h"

typedef enum rType {HEADER, 
    SUBMITTER, INDIVIDUAL, FAMILY, INVALID
    } RecordType;

typedef struct{
    char* id;
    void* locationOfRecord;
    bool used;
}Ref;

typedef struct{
    char* id;
    void * locationOfRefd;
}ReferencesUsed;

            









Field* createSubmitterField(char* line, int level);

//verify the header
Field* createHeaderField(char* line, int level);
Field* createIndiField(char* line, int level);
Header * createHeader(List* headerFieldList, List * subList);    
int findCharSet(char* value);
Ref *createRed(char * line);
    
int findCurrentLevel(char* line);

RecordType findRecordType(char* line);


#endif
