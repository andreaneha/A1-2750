#ifndef GEDCOMUTIL_H
#define GEDCOMUTIL_H

#include <ctype.h>
#include "GEDCOMparser.h"

typedef enum rType {HEADER, EVENT, FIELD, 
    SUBMITTER, INDIVIDUAL, FAMILY, INVALID
    } RecordType;

//verify the header
Field* createHeaderField(char* line, int level);
    
int findCurrentLevel(char* line);

RecordType findRecordType(char* line);



#endif
