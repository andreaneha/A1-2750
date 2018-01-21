#ifndef GEDCOMUTIL_H
#define GEDCOMUTIL_H

#include "GEDCOMparser.h"

typedef enum rType {HEADER, EVENT, FIELD, 
    SUBMITTER, INDIVIDUAL, FAMILY
    } recordType;

//verify the header
Header* createHeader(char* source, float gedcVersion, 
    CharSet encoding, Submitter* submitter, List otherFields);


#endif
