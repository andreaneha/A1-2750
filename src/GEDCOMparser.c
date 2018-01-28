#include <ctype.h>

#include "GEDCOMutilities.h"

GEDCOMerror createGEDCOM(char* fileName, GEDCOMobject** obj){
   // printf(">>>>>>%s\n", fileName);
    GEDCOMerror g;
    FILE* file;
    char line[200];
    bool headerExists = 0;
    bool subExists = 0;
    bool trExists=0;
    List headerList;
    List subList;
    List indiList;


    //check arguements
    if(fileName == NULL){
        g.type = 1;
        g.line = -1;
        return g;
    }
    if(obj== NULL){
         g.type = 2;        
         g.line = -1;
         return g;
    }


    //check file extension
    char * extension;

    int i = 0;
    while(fileName[i] != '\0'){
        if(fileName[i] == '.'){
            extension = &fileName[i];
            break;
        }
        i++;
        extension= "\0";
    }

    if(strcmp(extension, ".ged") != 0){
        g.type = 1;
        g.line = -1;
        return g;
    }

    file = fopen(fileName, "r");
    //check for null/non existant file
    if(file == NULL){
        g.type = 1;
        g.line = -1;
        return g;
    }

    int lineCounter=0;
    RecordType currentType=0;
    int currentLevel=0;
    int recordNum = -1;

    while(fgets(line,1000,file)!= NULL){
    //    printf("%s\n", line);
        // First Parse Header
        if(strcmp(line, "0 HEAD\n") == 0){
            if(!headerExists){
                // there has not been a header in the file yet. 
                // this is valid
                headerList = initializeList(printField, deleteField, compareFields);
                subList = initializeList(printField, deleteField, compareFields);
                indiList = initializeList(printField, deleteField, compareFields);
                currentType = 0;
                headerExists = 1;
                recordNum ++;
                lineCounter++;

                continue;

            }
            else{
                // there is a preexisting header
                // make sure to free all memory
                g.type = 2;
                g.line = lineCounter;
                return g;
            }

        }
        
        if(strcmp(line, "0 TRLR\n") == 0){
            subExists = 1;// ******* disable when this gets implemented
            if(headerExists && subExists && !trExists){
                Header* newHeader;
                newHeader = createHeader(&headerList);
                GEDCOMobject * object;
                object->header = newHeader;
                //printf(">>>>> hello\n");
                //both header and sub exists
                obj[recordNum] = object;
                trExists = 1;
                g.line = -1;
                g.type = 0;
                return g;
            }
            else{
                g.line = lineCounter;
                g.type = 1;
                return g;
            }
        }

        if(headerExists){
            int levelCheck;
            //keep track of the current level
                levelCheck = findCurrentLevel(line);

                    if(levelCheck == currentLevel+1){
                        currentLevel ++;
                    }
                    else if(levelCheck < currentLevel && levelCheck != 0){
                        currentLevel = levelCheck;
                    } 
                    else if(levelCheck == 0){
                        //figure out way to check the type of record
                        RecordType rt = findRecordType(line);
                        //printf(">>%d\n", rt);
                        if(rt == INVALID){
                            g.type = INV_RECORD;
                            g.line = lineCounter;
                            return g;
                        }

                        currentType = rt;
                        currentLevel = levelCheck;
                        continue;

                    }
                    else if(currentLevel == levelCheck){
                    }
                    else{
                        g.type = 2;
                        g.line = lineCounter;
                        return g;
                    }
                    if(currentType == 0){
                        //this is under the header
                        //printf("This is part of the header\n");
                        
                        Field * field;
                        field = createHeaderField(line, currentLevel);
                        if(field != NULL){
	                        insertBack(&headerList,field);
                        }
                        else{
                            printf("******\n");
                        }
                    }
                    else if(currentType == 1){
                        //this is part of the submitter
                        subExists = 1;
                        Field * field;
                        field = createSubmitterField(line, currentLevel);
                        if(field != NULL){
                            insertBack(&subList, field);
                            Node * node;
                            node = subList.head;

                        }
                        else{
                            printf("************\n");
                        }
                    }
                    else if(currentType == 2){
                        //this is part of individual
                        Field * field;
                        field = createIndiField(line, currentLevel);
                        if(field != NULL){
                            //printf("%s\n", field->tag);
                            insertBack(&indiList, field);
                            Node * node;
                            node = indiList.head;
                            Field * newField;
                            newField = (Field *) node->data;
                            if(newField->value == NULL){
                                strcpy(newField->value, " ");
                            }
                            //printf("%s:%s\n", newField->tag, newField->value);
                                                                          
                        }
                        else{
                            printf("************\n");
                        }

                    }

                }

        lineCounter++;

    }

    
    
    




    g.type = 1;
    g.line = 1; 
    return g;
}

void deleteField(void* toBeDeleted){
    Field* field;
    field = (Field *)toBeDeleted;
    
    if(field==NULL){
        //do Something
    }
    else{
        free(field->tag);
        free(field->value);
        free(field);
    }
            
}

int compareFields(const void* first,const void* second){
    // 0 false
    // 1 true
    Field* field1;
    Field* field2;
    field1 = (Field*)first;
    field2 = (Field*)second;
    int tagComp = strcmp(field1->tag,field2->tag);
    int valComp = strcmp(field1->value, field2->value);
    if(!tagComp && !valComp){
        return 1;
    }
    else{
        return 0;
    }
}

char* printField(void* toBePrinted){return NULL;}














