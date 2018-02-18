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

            








Field * createFamilyField(char* line, int currentLevel);
Field* createSubmitterField(char* line, int level);

//verify the header
Field* createHeaderField(char* line, int level);
Field* createIndiField(char* line, int level);
Header * createHeader(List* headerFieldList);    
int findCharSet(char* value);
Ref *createRed(char * line);
List * createIndiList(List** indiList, List** EventList);
    
int findCurrentLevel(char* line);

RecordType findRecordType(char* line);
bool isEvent(Field * field);


#endif
