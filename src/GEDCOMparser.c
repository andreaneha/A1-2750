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

    while(fgets(line,1000,file)!= NULL){
        // First Parse Header
        if(strcmp(line, "0 HEAD\n") == 0){
            if(!headerExists){
                // there has not been a header in the file yet. 
                // this is valid
                headerList = initializeList(printField, deleteField, compareFields);
                subList = initializeList(printField, deleteField, compareFields);
                currentType = 0;
                headerExists = 1;
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
                //both header and sub exists
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
	                        insertFront(&headerList,field);
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
                            insertFront(&subList, field);
                            Node * node;
                            node = subList.head;

                        }
                        else{
                            printf("************\n");
                        }
                    }




                }


        lineCounter++;

    }






    g.type = 0;
    g.line = 1; 
    return g;
}

void deleteField(void* toBeDeleted){}
int compareFields(const void* first,const void* second){return 0;}
char* printField(void* toBePrinted){return NULL;}