#ifndef GEDCOMUTIL_H
#define GEDCOMUTIL_H

#include <ctype.h>
#include "GEDCOMparser.h"

typedef enum rType {HEADER, EVENT, FIELD, 
    SUBMITTER, INDIVIDUAL, FAMILY, INVALID
    } RecordType;

//verify the header
Header* createHeader(char* source, float gedcVersion, 
    CharSet encoding, Submitter* submitter, List otherFields);
    
int findCurrentLevel(char* line);

RecordType findRecordType(char* line);



#endif
