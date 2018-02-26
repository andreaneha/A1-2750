#ifndef GEDCOMUTIL_H
#define GEDCOMUTIL_H

#include <ctype.h>
#include "GEDCOMparser.h"

typedef enum rType {HEADER, 
    SUBMITTER, INDIVIDUAL, FAMILY, INVALID
    } RecordType;

typedef struct{
    char *id;
    void* locationOfRecord;
    bool used;
}Ref;

typedef struct{
    char *id;
    void ** locationOfRefd;
    bool used;
}ReferencesUsed;

typedef struct{
    List* ref;
    List * refUsed;
    List * headerList;
    List * subList;
    List ** indiList;
    int indiLen;
    List ** eventList;
    int eventLen;
    List ** familyList;
    int familyLen; 
    FILE* file;
}HeapPointer;



HeapPointer * createHeapPointer(FILE * file);

void purge(HeapPointer * hp);
void deleteRefList(void* toBeDeleted);
void deleteRefUsedList(void* toBeDeleted);

List * createEvents(List **eventList, int len, List * refUsedList);
void eventHandling(List * individualList, List * eventList);
void updateListTags(List * refList, List ** oldList, int len, List* newList);
List * createFamilies(List **familyList, List * refUsedList, int famlen);           
List * createIndividuals(List **indiList, List * refUsedList, int indilen);
void updateTag(List * refList, void * oldData, void * newData);
Submitter * createSubmitter(List * subList, List * refUsedList);
ReferencesUsed * createNewRef(char* id, void* locationOfRefd);
Ref * findRef(List * refList, char* refID);
void referenceHandle(Field * field, List* refUsedList);
void  printEventFields(List * list);
Field * createFamilyField(char* line, int currentLevel);
Field* createSubmitterField(char* line, int level);
int link(List * objectRef, List * refUsedLocation);

//verify the header
Field* createHeaderField(char* line, int level);
Field* createIndiField(char* line, int level);
Header * createHeader(List* headerFieldList, List * refUsedList);    
int findCharSet(char* value);
Ref *createRed(char * line);
List * createIndiList(List** indiList, List** EventList);
    
int findCurrentLevel(char* line);

RecordType findRecordType(char* line);
bool isEvent(Field * field);
void printIndividualList(List * indiList);

#endif
