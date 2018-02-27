#include <ctype.h>

#include "GEDCOMutilities.h"

GEDCOMerror createGEDCOM(char* fileName, GEDCOMobject** obj){
   // printf(">>>>>>%s\n", fileName);
    GEDCOMerror g;
    FILE* file;
    char rawline[200];
    bool headerExists = 0;
    bool subExists = 0;
    bool trExists=0;
    bool isField=0;
    List headerList;
    List subList;
    List **indiList = malloc(sizeof(List*)*200);
    List refList; //id tags of each record
    List refUsedList; //where id is used
    int indilen = -1;
    int eventLen= -1;
    int famLen = -1;
    int fieldLevel = 0;
    List **EventList = malloc(sizeof(List*)*200);
    List **familyList = malloc(sizeof(List*)*200);
    HeapPointer *hp;

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

    while(fgets(rawline,1000,file)!= NULL){
    //    printf("%s\n", linlocationOfRefde);
        // First Parse Header
        printf("%s", rawline);
        
        int lineLen = strlen(rawline);
        for(int i = 0; i<lineLen; i++){
            char a = rawline[i];
            if(a == '\n' || a == '\r'){
                rawline[i] = '\0';
            }
        }
        lineLen = strlen(rawline);
        char line[lineLen+1];
        strcpy(line,rawline);

        if(strcmp(line, "0 HEAD") == 0){
            if(!headerExists){
                // there has not been a header in the file yet. 
                // this is valid
                headerList = initializeList(printField, deleteField, compareFields);
                subList = initializeList(printField, deleteField, compareFields);
                //familyList = initializeList(printField, deleteField, compareFields);
                refList = initializeList(NULL, deleteRefList, NULL);
                refUsedList = initializeList(NULL, deleteRefUsedList, NULL); 
                hp = createHeapPointer(file);
                hp->headerList = &headerList;
                hp->subList = &subList;
                hp-> ref= &refList;
                hp-> refUsed= &refUsedList;
                hp->indiList = indiList;
                hp->eventList = EventList;
                hp->familyList = familyList;
                hp->indiLen = -1;
                hp->eventLen = -1;
                hp->familyLen = -1;

                currentType = 0;
                headerExists = 1;
                recordNum ++;
                lineCounter++;

                continue;
            }
            else{
                purge(hp);
                g.type = 2;
                g.line = lineCounter;
                return g;
            }

        }
        
        if(strcmp(line, "0 TRLR") == 0){
            subExists = 1;// ******* disable when this gets implemented
            if(headerExists && subExists && !trExists){
                Header* newHeader;
                Submitter * newSubmitter;
                //List **individualListp = malloc(sizeof(List));
                List * individualList;
                List * allFamilyList;
                List * allEventList;
                newHeader = createHeader(&headerList, &refUsedList);
                newSubmitter = createSubmitter(&subList, &refUsedList);
                indilen++;
                eventLen++;
                famLen++;
                indiList[indilen] = NULL;
                EventList[eventLen]=NULL;
                familyList[famLen] = NULL;
                GEDCOMobject * object;
                object = malloc(sizeof(GEDCOMobject));
                individualList = createIndividuals(indiList, &refUsedList, indilen);
                allFamilyList = createFamilies(familyList, &refUsedList, famLen);
                allEventList = createEvents(EventList, eventLen, &refUsedList);

                //printf(">>>>> hello %s\n", object.header);
                //printIndividualList(individualList);
                
                //both header and sub exists
                //List * ind = createIndiList(indiList, EventList);
                //object->individuals = *ind;
                obj[recordNum] = object;
                trExists = 1;
                g.line = -1;
                g.type = 0;
                // *********************************************
                //LAST MINUTE THINGS

                updateTag(&refList, &subList, newSubmitter);
                updateListTags(&refList, indiList, indilen, individualList);
                updateListTags(&refList, familyList, famLen, allFamilyList);


                eventHandling(individualList, allEventList);

                
                link(&refList, &refUsedList);

                object->header = newHeader;
                object->families = *allFamilyList;
                object->individuals = *individualList;
                object->submitter = newSubmitter;
                
                

                //for(int i = 0; i < eventLen ; i++){
                //    printf("--------------\n");
                //    printEventFields(EventList[i]);
                //
                //}
                
                obj = &object;
               
               















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
                            purge(hp);
                            g.type = INV_RECORD;
                            g.line = lineCounter;
                            return g;
                        }
                        else if(rt == 2){
                            indilen++;
                            List *newList = malloc(sizeof(List));
                            *newList = initializeList(printField, deleteField, compareFields);
                            indiList[indilen] = newList;
                            hp->indiLen = indilen;
                        }
                        else if(rt == 3){
                            //family
                            famLen++;
                            List newList = initializeList(printField, deleteField, compareFields);
	                        familyList[famLen] = &newList;
                            hp->familyLen = famLen;
                        //    printf("%s", line);
                            
                        }
                        currentType = rt;
                        currentLevel = levelCheck;
                        Ref * ref;
                        ref = createRed(line);
                        if (ref == NULL){
                            purge(hp);
                            g.type = INV_RECORD;
                            g.line = lineCounter;
                            return g;
                        }
                        else{//CHANGE THIS TO JUST ELSE
	                        if(rt == 1){ref->locationOfRecord = &subList;} 
                            else if(rt == 2){ref->locationOfRecord = indiList[indilen];}
                            else if(rt == 3){ref->locationOfRecord = familyList[famLen];}

	                        insertBack(&refList, ref);
                            continue;
                        }
                    }
                    else if(currentLevel == levelCheck){
                    }
                    else{
                        purge(hp);
                        g.type = 2;
                        g.line = lineCounter;
                        return g;
                    }

                    if(currentType == 0){
                        //this is under the header
                       // printf("This is part of the header\n");
                        
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
                            //Node * node;
                            //node = subList.head;

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
                            if(isEvent(field) || isField){
                                if(!isField){
                                    deleteField(field);
                                    fieldLevel = currentLevel;
                                    eventLen++;
                                    List *newList = malloc(sizeof(List));


                                    if(newList == NULL){
                                        free(field);
                                        purge(hp);
                                        g.type = INV_RECORD;
                                        g.line = lineCounter;
                                        return g;


                                    }
                                    *newList = initializeList(printField,
                                        deleteField, compareFields);
                                        //indiList[indilen] = malloc(sizeof(List*));
                                    EventList[eventLen] = newList;
                                    hp->eventLen = eventLen;
                                    //insertBack(EventList[eventLen], field);
                                    isField =1;
                                }
                                else if(currentLevel == fieldLevel && isField){
                                    //deleteField(field);
                                    Field * field2 = malloc(sizeof(Field));
                                    if(field2 == NULL){
                                        purge(hp);
                                        g.type = INV_RECORD;
                                        g.line = lineCounter;
                                        return g;
                                    }
                                    field2->tag = malloc(sizeof(char)*(6));
                                    strcpy(field2->tag, "event");
                                    char buffer[11];
                                    sprintf(buffer, "index=%d", eventLen);

                                    field2->value = malloc(sizeof(char)*(strlen(buffer) + 1));
                                    strcpy(field2->value, buffer);
                                    insertBack(indiList[indilen],field2);
                                    isField = 0;
                                    //insertBack(indiList[indilen],field);

                                }else
                                {
                                    insertBack(EventList[eventLen], field);

                                }
                            }
                            if(!isField){
                                hp->indiLen = indilen;
                                insertBack(indiList[indilen], field);
                             }

                           // Node * node;
                           // node = indiList.head;
                           // Field * newField;
                           // newField = (Field *) node->data;
                           // if(newField->value == NULL){
                           //     strcpy(newField->value, " ");
                           // }
                           // //printf("%s:%s\n", newField->tag, newField->value);
                           // //List * newList;

                            
                        }
                        else{
                            deleteField(field);
                            purge(hp);
                            g.type = INV_RECORD;
                            g.line = lineCounter;
                            return g;

                            

//                            printf("1************\n");
                        }


                    }
                    else if(currentType ==3){
                        Field * field;
                        field = createFamilyField(line, currentLevel); 
                        if(field != NULL){
                        //printf("%s\n", field->tag);
                            if(isEvent(field) || isField){
                                if(!isField){
                                    fieldLevel = currentLevel;
                                    eventLen++;
                                    List *newList = malloc(sizeof(List));
                                    *newList = initializeList(printField,
                                    deleteField, compareFields);
                                    //indiList[indilen] = malloc(sizeof(List*));
                                    EventList[eventLen] = newList;
                                    insertBack(EventList[eventLen], field);
                                    isField =1;
                                }
                                else if(currentLevel == fieldLevel && isField){
                                    Field * field = malloc(sizeof(field));
                                    field->tag = malloc(sizeof(char)*6);
                                    field->value = malloc(sizeof(char)*20);
                                    strcpy(field->tag, "event");
                                    char buffer[20];
                                    sprintf(buffer, "index=%d", eventLen);
                                    strcpy(field->value, buffer);
                                    insertBack(familyList[famLen],field);
                                    isField = 0;
                                                                                 
                                }else
                                {
                                    insertBack(EventList[eventLen], field);
                                }
                            }
                            if(!isField){
                                insertBack(familyList[famLen], field);
                            }
                       // Node * node;
                       // node = indiList.head;
                       // Field * newField;
                       // newField = (Field *) node->data;
                       // if(newField->value == NULL){
                       //     strcpy(newField->value, " ");
                       // }
                       // //printf("%s:%s\n", newField->tag, newField->value);
                       // List * newList;
                        
                        }
                        else{
                            printf("************\n");
                        }
                    }


                }

        lineCounter++;


    }
    purge(hp);
    g.type = 1;
    g.line = 1; 
    return g;
}

void deleteField(void* toBeDeleted){
    printf("..........\n");
    Field* field;
    field = (Field *)toBeDeleted; 
    if(field==NULL){
        //do Something
    }
    else{
        if(field->tag != NULL || strcmp(field->tag,"")!=0){
            printf("%s\n", field->tag);
            free(field->tag);
        }

        if(field->value != NULL|| strcmp(field->value, "") !=0){
            printf("%s\n", field->value);
            free(field->value);
        }
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
    int tagComp = 0;
    int valComp = 0;
    bool nullFound = 0;
    if(field1->tag == NULL || field2->tag == NULL){
        if(field1->tag == NULL && field2->tag == NULL){
            tagComp = 1;
        }
        nullFound = 1;
    }
    if(field1->value == NULL || field2->value == NULL){
        if(field1->value == NULL && field2->value == NULL){
            valComp = 1;
        }
        nullFound = 1;
    }

    if(!nullFound){
        tagComp = strcmp(field1->tag,field2->tag);
        valComp = strcmp(field1->value, field2->value);
    }
    if(!tagComp && !valComp){
        return 0;
    }
    else{
        return 1;
    }
}

char* printField(void* toBePrinted){
    Field * field;
    char *fieldstr = malloc(sizeof(char) * 160);
    field = (Field*) toBePrinted;
    sprintf(fieldstr, "%s=%s\n", field->tag, field->value);
    return fieldstr;
}













