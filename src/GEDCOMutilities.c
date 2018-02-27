#include <ctype.h>
#include "GEDCOMutilities.h"


void purge(HeapPointer *hp){
    if(hp->ref != NULL){
        clearList(hp->ref);
        hp->ref = NULL;
    }

    if(hp->refUsed != NULL){
        clearList(hp->refUsed);
        hp->refUsed = NULL;
    }

    if(hp->headerList != NULL){
        clearList(hp->headerList);
        hp->headerList = NULL;
    }
    if(hp->subList != NULL){
        clearList(hp->subList);
        hp->subList = NULL;

    }
    if(hp->file != NULL){
        fclose(hp->file);
    }
    if(hp->indiLen >-1){
        for(int i = 0; i<= hp->indiLen; i++){
            List * currentList = hp->indiList[i];
            clearList(currentList);
            free(currentList);
        }
    }
    if(hp->eventLen >-1){
        for(int i = 0; i<= hp->eventLen; i++){
            List * currentList = hp->eventList[i];
            clearList(currentList);
            free(currentList);
        }
    }
    if(hp->familyLen >-1){
        for(int i = 0; i<= hp->familyLen; i++){
            List * currentList = hp->familyList[i];
            clearList(currentList);
        }
    }
    if(hp->indiList != NULL){
        free(hp->indiList);
        hp->indiList = NULL;
    }
    if(hp->eventList != NULL){
        free(hp->eventList);
        hp->eventList = NULL;
    }
    if(hp->familyList != NULL){
        free(hp->familyList);
        hp->familyList = NULL;
    }



    free(hp);


}






void deleteRefList(void* toBeDeleted){
    Ref * ref = toBeDeleted;
    if(ref->id != NULL){
        free(ref->id);
    }
    free(ref);
}

void deleteRefUsedList(void* toBeDeleted){
    ReferencesUsed *ref = toBeDeleted;
    if(ref->id != NULL){
        free(ref->id);
    }
    free(ref);
}

HeapPointer * createHeapPointer(FILE * file){
    HeapPointer* hp = malloc(sizeof(HeapPointer));

    hp->ref = NULL;
    hp->refUsed = NULL;
    hp->headerList= NULL;
    hp->subList= NULL;;
    hp->indiList=NULL;
    hp->indiLen = -1;
    hp->eventList = NULL;
    hp->eventLen = -1;
    hp->familyList = NULL;
    hp->familyLen = -1;
    hp->file = NULL;
    if(file != NULL){
        hp->file = file;
    }

return hp;



}






List * createEvents(List **eventList, int len, List*refUsedList){
    List * listOfEvents = malloc(sizeof(List));
    *listOfEvents = initializeList(NULL, NULL, NULL);
    List * currentEvent = NULL;
    for(int i = 0; i<len; i++){
        currentEvent = eventList[i];
    	ListIterator iter = createIterator(*currentEvent);
        Field * field;
        Event * event = malloc(sizeof(Event));
    
        field = (Field*)nextElement(&iter);
        strcpy(event->type, field->tag);
        Field *fieldTemp = field;
        field = (Field*)nextElement(&iter);
        deleteDataFromList(currentEvent, fieldTemp);
        bool delField;
        while(field != NULL){
            referenceHandle(field, refUsedList);
            //nameHandle(),
            delField = 0;
            if(strcmp(field->tag, "DATE")==0){
                char* date = malloc(sizeof(field->value));
                strcpy(date, field->value);
                event->date = date;
                delField = 1;

            }
            else if(strcmp(field->tag, "PLAC")==0){
                char* place = malloc(sizeof(field->value));
                strcpy(place, field->value);
                event->place = place;
                delField = 1;
            }

            if(delField){
                Field *fieldTemp = field;
                field = (Field*)nextElement(&iter);
                deleteDataFromList(currentEvent, fieldTemp);
                continue;
            }
            field = (Field *)nextElement(&iter);
        }
        event->otherFields = *currentEvent;
        insertBack(listOfEvents, event);
    } 
    return listOfEvents;
}

void eventHandling(List * individualList, List * eventList){
    List  indiEventList;
    Field * currentField;
    Node* oldField;
    int index;
    //individual iterator
    ListIterator indIter = createIterator(*individualList);
    Individual* indi = nextElement(&indIter);
    while(indi != NULL){
        indiEventList = (*indi).events;
        oldField = indiEventList.head;
        ListIterator iter = createIterator(indiEventList);
        //currentField = iter.current->data;
        currentField = nextElement(&iter);
//        printf("person's name: %s\n", indi->givenName);
        while(currentField != NULL){
            char* temp = &(currentField->value)[6];
            index = atoi(temp);
//            printf("%d\n", index);
            Event * eventFound;
            ListIterator eventIter = createIterator(*eventList);
            //we found an event and we know it's index
            for(int i = 0; i <= index; i++){
                eventFound = nextElement(&eventIter);                
            }
            oldField->data = eventFound;
            free(currentField->tag);
            free(currentField->value);
            free(currentField);
            oldField = oldField->next;
            currentField = nextElement(&iter);
        }
        indi = nextElement(&indIter);
    }
}

void updateListTags(List * refList, List ** oldList, int len, List* newList){
    ListIterator iter = createIterator(*newList);
    void * currentNewList = nextElement(&iter);
    for(int i = 0; i<len ; i++){
        updateTag(refList, oldList[i], currentNewList);
        currentNewList = nextElement(&iter);
    }
}

List * createFamilies(List **famList, List * refUsedList, int famLen){
    List * listOfFamilies = malloc(sizeof(List));
    *listOfFamilies = initializeList(NULL, NULL, NULL);
    List * currentFamily = NULL;
    for(int i = 0; i<famLen; i++){
        currentFamily = famList[i];
        ListIterator iter = createIterator(*currentFamily);
        Field * field;
        Family * family = malloc(sizeof(Family));
        List * children = malloc(sizeof(List));
        *children = initializeList(NULL,NULL,NULL);
                                                                
        field = (Field*)nextElement(&iter);
                                                                
        bool delField;
        while(field != NULL){
            referenceHandle(field, refUsedList);
            //nameHandle(),
            delField = 0;
            if(strcmp(field->tag, "HUSB")==0){
                ReferencesUsed * ru = getFromBack(*refUsedList);
                ru->locationOfRefd = (void*) &family->husband;
                delField = 1;
            }
            else if(strcmp(field->tag, "WIFE")==0){
                ReferencesUsed * ru = getFromBack(*refUsedList);
                ru->locationOfRefd = (void*) &family->wife;
                delField = 1;
            }
            else if(strcmp(field->tag, "CHIL")==0){
                Individual * child = NULL;
	            insertBack(children, &child);
                ReferencesUsed * ru = getFromBack(*refUsedList);
	            ru->locationOfRefd = &children->tail->data;
                delField = 1;
            }
            if(delField){
                Field *fieldTemp = field;
                field = (Field*)nextElement(&iter);
                deleteDataFromList(currentFamily, fieldTemp);
                continue;
            }
            field = (Field *)nextElement(&iter);
        }
        family->children = *children;
        insertBack(listOfFamilies, family);
    }
    return listOfFamilies;
}





List * createIndividuals(List **indiList, List * refUsedList, int indilen){
    List * listOfIndividuals = malloc(sizeof(List));
    *listOfIndividuals = initializeList(NULL, NULL, NULL);
    List * currentIndividual = NULL;
    for(int i = 0; i<indilen; i++){
        currentIndividual = indiList[i];
    	ListIterator iter = createIterator(*currentIndividual);
        Field * field;
        Individual * indi = malloc(sizeof(Individual));
        
        List * listOfEvents = malloc(sizeof(List));
        
        *listOfEvents = initializeList(NULL, NULL, NULL);

    
        field = (Field*)nextElement(&iter);

        bool delField ;
        char *name = malloc(sizeof(char)*200);
    
        while(field != NULL){
            printf("%s\n", field->value);
            referenceHandle(field, refUsedList);
            //nameHandle(),
            delField = 0;
            if(strcmp(field->tag, "NAME")==0){
                strcpy(name, field->value);
                delField = 1;
            }else if(strcmp(field->tag, "event")==0){
                Field * newField = malloc(sizeof(Field));
                newField->tag = malloc(sizeof(char)*6);
                newField->value = malloc(sizeof(char)*15);
                strcpy(newField->tag, field->tag);
                strcpy(newField->value, field->value);
                insertBack(listOfEvents, newField);
                delField = 1;
            }



            
            if(delField){
                Field *fieldTemp = field;
                field = (Field*)nextElement(&iter);
                deleteDataFromList(currentIndividual, fieldTemp);
                continue;
    
            }
            field = (Field *)nextElement(&iter);
    
        }
        iter.current = currentIndividual->head;
        field = (Field*)nextElement(&iter);
    
        while(field!=NULL){
            field = (Field*) nextElement(&iter);
        }
         
        indi->givenName = name;
        indi->otherFields = *currentIndividual;
        indi->events = *listOfEvents;
        insertBack(listOfIndividuals, indi);

   } 
	return listOfIndividuals;
}

void updateTag(List * refList, void * oldData, void * newData){
    ListIterator iter = createIterator(*refList);
    Ref * current = (Ref *) nextElement(&iter);
    while(current != NULL){
        if(current->locationOfRecord == oldData){
            current->locationOfRecord = newData;
            current = NULL;
        }
        else{current = (Ref*) nextElement(&iter);};
    }
}

Submitter * createSubmitter(List * subList, List * refUsedList){
    Submitter * sub = NULL;
    //printEventFields(subList);
    if(subList == NULL){
        return sub;
    }
	ListIterator iter = createIterator(*subList);
    Field * field;    

    field = (Field*)nextElement(&iter);
    

    bool delField ;
    bool addFound = 0;
    bool nameFound = 0;
    char name[61];
    char address[200];
    int len = 0;

    while(field != NULL){
        referenceHandle(field, refUsedList);
        delField = 0;
        if(strcmp(field->tag, "NAME")==0){
            strcpy(name, field->value);
            nameFound = 1;
            delField = 1;
        }
        else if(strcmp(field->tag, "ADDR")==0){
            len = strlen(field->value);
            strcpy(address, field->value);
            addFound = 1;
            delField = 1;
        }
        if(delField){
            Field *fieldTemp = field;
            field = nextElement(&iter);
            deleteDataFromList(subList, fieldTemp);
            continue;

        }
        field = (Field *)nextElement(&iter);

    }
    iter.current = subList->head;
    field = (Field*)nextElement(&iter);

    while(field!=NULL){
        field = (Field*) nextElement(&iter);
    }

    sub = malloc(sizeof(Submitter) + (len+1)*sizeof(char));
    if(addFound){
        strcpy(sub->address, address);
    }if(nameFound){
        strcpy(sub->submitterName, name);
    }
	sub->otherFields = *subList;
    return sub;
}

Ref * findRef(List * refList, char* refID){
    Ref * refFound = NULL;
    ListIterator iter = createIterator(*refList);
    Ref * ref = (Ref *) nextElement(&iter);
    while(ref != NULL){
        if(strcmp(ref->id, refID) == 0){
            refFound = ref;
            break;
        }
        ref = (Ref*) nextElement(&iter);

    }

    return refFound;
}

//this function returns error = 1 is something goes wrong
int link(List * objectRef, List * refUsedLocation){
    int error = 1;
    
    ListIterator iter = createIterator(*refUsedLocation);
    ReferencesUsed * refUsed = (ReferencesUsed *) nextElement(&iter);
    while(refUsed != NULL){
        Ref * ref = findRef(objectRef, refUsed->id);
        if(ref != NULL){
            *(refUsed ->locationOfRefd) = ref->locationOfRecord;
            ref->used = 1;
            refUsed->used = 1;


        }else{
            //ERROR
        }
        refUsed = (ReferencesUsed*)nextElement(&iter);
    }
    return error;
}

ReferencesUsed * createNewRef(char* id, void* locationOfRefd){
    ReferencesUsed *ru = NULL;
    if(id != '\0' && locationOfRefd!= NULL){
        ru = malloc(sizeof(ReferencesUsed));
        ru-> used = 0;
        ru->id = malloc(sizeof(char)*(strlen(id)+1));
        strcpy(ru->id, id);
        ru->locationOfRefd = locationOfRefd;
    }
    return ru;
}

void referenceHandle(Field * field, List* refUsedList){
    int len = strlen(field->value);
    char * value = field->value;
    int openIndex = -1;
    int closeIndex = -1;
    for(int i = 0; i<len; i++){
        if(value[i] == '@'){
            if(openIndex == -1 && closeIndex == -1){
                openIndex = i;
            }else if(openIndex != -1 && closeIndex == -1){
                closeIndex = i;
            }else if(openIndex != -1 && closeIndex != -1){
                //something's wrong
            }
        }
    }

    if(openIndex == -1 && closeIndex == -1){
        //no tags
    //    printf("%s: no tags\n", value);
        return;
    }
    //this is a tag
    //-----create new refUsed item

    char valcopy [len];
    char *refIDstr;
    //int refid;
    ReferencesUsed* ru;

    strcpy(valcopy, value);
    refIDstr = &valcopy[openIndex+1];
    valcopy[closeIndex] = '\0';
    int index = 0;
    //while(refIDstr[index] != '\0'){
    //    if(!isdigit(refIDstr[index])){
    //        return;
    //    }
    //    index++;
    //}

    //refid = atoi(refIDstr);
//    printf(">>%d\n", refid);

	ru = createNewRef(refIDstr, &field->value);
    free(field->value);
    field->value = NULL;
    if(refUsedList != NULL){
        insertBack(refUsedList, ru);
    }
    return;
 }

void printEventFields(List * list){
    int len = list->length;
    ListIterator iter = createIterator(*list);
    Field * field = (Field*) nextElement(&iter);
    while(field!= NULL){
        printf("tag=%s     value=%s\n", field->tag, field->value);
        field =(Field*) nextElement(&iter);

    }

    printf("%d\n", len);
}

Field * createFamilyField(char* line, int currentLevel){
    //Field * field = NULL;
    //printf("%s :%d\n", line, currentLevel);

    char * tags []= { 
        "ADDR", "ADR1", "ADR2", "ADR3", "AGE", "AGNC", "ANUL", "CAUS", "CENS", "CHIL",
        "CITY", "CONC", "CONT", "CTRY", "DATA", "DIV", "DIVF", "EMAIL", "ENGA", "EVEN",
        "FAX", "FILE", "FONE", "FORM", "HUSB", "LATI", "LONG", "MAP", "MARB", "MARC",
        "MARL", "MARR", "MARS", "NCHI", "NOTE", "OBJE", "PAGE", "PHON", "PLAC", "POST",
        "QUAY", "REFN", "RELI", "RESI", "RESN", "ROLE", "ROMN", "SLGS", "SOUR", "STAE",
        "STAT", "STRY", "SUBM", "TEMP", "TEXT", "TITLE", "TYPE", "WIFE", "WILD", "WWW", 
        "DATE"
     };
    
    int len = strlen(line);                                            
    char tag[5];                                                   
    char val[len];                                                 
    char *data;                                                    
    int tagIndex = 0;
    int spaceIndex= 0;
    strcpy(val, line);
    bool whiteSpace = 0;
    int lengthOfTag=0;
    bool valExist=0;
    if(val[0] != '\t' && val[0] != ' '){
    spaceIndex++;
    }
                                          
    for(int i=0; i<len;i++){
    if(val[i] == '\t' || val[i] == ' '){
        if(whiteSpace){
            continue;
        }
        else{
            spaceIndex++;
            whiteSpace = 1;
            }
        }
        else{
             if(whiteSpace){
                 if(spaceIndex == 2){
                     lengthOfTag++;
                 }
                 whiteSpace = 0;
             }
             else{
                 if(spaceIndex ==2){
                     lengthOfTag++;
                     continue;
                 }
             }
         }
    }
 
    
    for(int i = 0; i<len; i++){
        char c = val[i];
         if(c != ' '&& c!= '\t' && c!= '\n'){
            if(isdigit(c) && tagIndex == 0){
                continue;
            }
            if(tagIndex<lengthOfTag){
                tag[tagIndex] = c;
                tagIndex++;
                if(tagIndex == lengthOfTag){
                    tag[tagIndex] = '\0';
                    tagIndex++;
                    continue;
                }
                continue;
            }
        }
        if(tagIndex == lengthOfTag+1 && c !=' ' ){
             data = &val[i];
             valExist = 1 ;
             tagIndex++;
        }
        else if(c == '\n'){
            val[i] = '\0';
        }
                                                      
     }
     tag[tagIndex] = '\0';
     bool tagFound = 0;
     for(int i=0; i<61; i++){
         if(strcmp(tag,tags[i])==0){
             tagFound = 1;
         }
     }
 
     Field *newField = malloc(sizeof(Field));
     if(tagFound){
     ///// make sure everything is valid
         //printf("%d\n", (int) strlen(data));
         char * newTag = malloc(sizeof(char)*(strlen(tag)+1));
         if(!valExist){
             //strcpy(data, " ");
             data = " ";
         }
         char * newValue = malloc(sizeof(char)*(strlen(data)+1));
         strcpy(newValue, data);
         newField->value = newValue;
 
         strcpy(newTag, tag);
         newField->tag = newTag;
     }
     else{
         return NULL;
     }
     //printf("**%s**\n", newField->tag);
     return newField;
 }


bool isEvent(Field * field){
    char * tags[] = {"BIRT", "CHR" , "DEAT", "BURI", "CREM", "ADOP", "BAPM", "BARM" , "BASM", 
              "BLES", "CHRA", "CONF", "FCOM", "ORDN", "NATU", "EMIG", "IMMI", "CENS", "PROB", 
              "WILL", "GRAD", "RETI","EVEN"}; //51
     
    char fieldTag[5];
    char dummy = '\0';
    //fieldTag = &dummy;
     strcpy(fieldTag, field->tag);
     fieldTag[strlen(field->tag)] = '\0';
    //char *fieldVal = field->value;
    bool eventFound = 0;

    for(int i = 0; i<23;i++){
        if(strcmp(fieldTag, tags[i])==0){
            eventFound = 1;
            break;
        }

    }
    return eventFound;
}

List * createIndiList(List** indiList, List** EventList){

    int listLen = 0;
    int listIndex = 0;
    //List * ret = NULL;

    while(indiList[listIndex]!=NULL){
        listLen++;
        listIndex++;
    }

    //initialize list of individuals
    List * listOfIndividuals=malloc(sizeof(List));
    *listOfIndividuals = initializeList(NULL, NULL, compareFields);

    for(int i=0; i<listLen; i++){
        List * currentList;
        currentList = indiList[i];
        ListIterator iter = createIterator(*currentList);
        Field * currentNode;
	    currentNode = nextElement(&iter);
        Individual * individual= malloc(sizeof(Individual));
        bool delNode;
        bool eventInitialized = 0;

        while (currentNode != NULL){
            delNode = 0;
            if(strcmp(currentNode->tag, "NAME") == 0){
                //parse first and last name
                individual->givenName = currentNode->value;
                delNode = 1;
            }
            else if(strcmp(currentNode->value, " ") == 0){
                delNode = 1;
            }
            else if(strcmp(currentNode->tag,"event")==0){
                char val[20];
                int index;
                strcpy(val, currentNode->value);
                index = atoi(&val[6]);
                if(!eventInitialized){
                    eventInitialized =1;
                    List* list = malloc(sizeof(List)); 
                    *list = initializeList(printField, deleteField, compareFields);

	                individual->events = *list;
                }
                Event * event = malloc(sizeof(event));
                event->otherFields = *EventList[index];
               //FILL IN EVENTS FIELDS 
	            insertBack(&individual->events, event);
                delNode =1;

            }

           //detect events and families

           if(delNode){
              Field *fieldTemp = currentNode;
              currentNode = nextElement(&iter);
              deleteDataFromList(currentList, fieldTemp);
            }
            else{
                currentNode = nextElement(&iter);
           }
	       individual->otherFields = *currentList;
           insertBack(listOfIndividuals, individual);

        }


        
    }

	return listOfIndividuals;
}

Ref *createRed(char * line){
  //  printf("%s\n", line);
    Ref * ref = NULL;
    bool open = 0;
    bool close = 0;
    int letterCount = 0;
    int linelen;
    linelen = strlen(line);
    char * at;

    for(int i=0; i<linelen; i++){
        if(line[i] == '@' && !open){
            open = 1;
            if(line[i+1] != '\0');
                at = &line[i+1];
            continue;
        }
        else if(line[i] == '@' && open){
            close = 1;
            line[i] = '\0';
            continue;
        }
        else if(line[i] != '@' && open && !close){
            letterCount++;
        }
    }

    if(open && close){
        char * id;
//        printf(">>%d\n", letterCount);
        id = malloc((sizeof(char))*letterCount+1);
        if(id == NULL){
            return ref;
        }
        ref = malloc(sizeof(Ref));
        if(ref == NULL){
            free(id);
            return ref;
        }
        strcpy(id, at);
        ref->id = id;
    }


return ref;

}

int findCharSet(char* value){
    int charVal;
    
    if(strcmp(value,"ANSEL")==0){
        charVal = 0;
    }else if(strcmp(value,"UTF8")==0){
        charVal = 1;
    }else if(strcmp(value,"UNICODE")==0){
        charVal = 2;
    }else if(strcmp(value,"ASCII")==0){
        charVal = 3;
    }else{
        charVal = 4;
    }

return charVal;
}

Header * createHeader(List* headerFieldList, List * refUsedList){

	ListIterator iter = createIterator(*headerFieldList);
    Field * field;    

    field = (Field*)nextElement(&iter);
    
    Header * header;
    header = malloc(sizeof(Header));

    bool delField ;

    while(field != NULL){
        referenceHandle(field, refUsedList);
        delField = 0;
        if(strcmp(field->tag, "SOUR")==0){
            strcpy(header->source, field->value);
            delField = 1;
        }
        else if(strcmp(field->tag, "GEDC")==0){
            field = (Field *)nextElement(&iter);
            header->gedcVersion = atof(field->value);
            delField = 1;
        }
        else if(strcmp(field->tag, "CHAR")==0){
            header->encoding = (CharSet)findCharSet(field->value);
            delField = 1;
        }
        else if(strcmp(field->tag, "SUBM")==0){
            ReferencesUsed * ru;
            //submitter pointer goes here
            header->submitter = NULL;
	        ru = (ReferencesUsed*) getFromBack(*refUsedList);
            ru->locationOfRefd = (void*) &(header->submitter);
            delField = 1;
        }

        if(delField){
            Field *fieldTemp = field;
            field = nextElement(&iter);
            deleteDataFromList(headerFieldList, fieldTemp);
            continue;

        }
        field = (Field *)nextElement(&iter);

    }
    iter.current = headerFieldList->head;
    field = (Field*)nextElement(&iter);

    while(field!=NULL){
        field = (Field*) nextElement(&iter);
    }

    header->otherFields = *headerFieldList;


    return header;
}

Field* createIndiField(char* line, int level){
    char * tags[] = {"ADDR", "ADOP", "ADR1", "ADR2", "ADR3", "AFN", "AGE", "AGNCY", "ALLA",    
            "ANCI", "ASSO", "BAPL", "BAPM", "BARM", "BASM", "BIRT", "BLES", "BURI",
            "CAST", "CAUS", "CENS", "CHAN", "CHR",  "CHRA", "CITY", "CONC", "CONF", 
            "CONL", "CONT", "CREM", "CTRY", "DATA", "DATE", "DEAT", "DESI", "DSCR", 
            "EDUC", "EMAIL", "EMIG", "ENDL", "EVEN", "FACT", "FAMC", "FAMS", "FAX", 
            "FCOM", "FILE", "FONE", "FORM", "GIVN", "GRAD", "IDNO", "IMMI", "LATI", 
            "LONG", "MAP",  "MEDI", "NAME", "NATI", "NATU", "NCHI", "NMR",  "NOTE", 
            "NPFX", "NSFX", "OBJE", "OCCU", "ORDN", "PAGE", "PEDI", "PHON", "PLAC", 
            "POST", "PROB", "PROP", "QUAY", "REFN", "RELA", "RELI", "RESN", "RETI", 
            "RFN",  "RIN",  "ROLE", "ROMN", "SLGC", "SOUR", "SPFX", "SSN",  "STAE", 
            "STAT", "SUBM", "SURN", "TEMP", "TEXT", "TIME", "TITL", "TYPE", "WILL", 
            "WWW", "SEX"}; //101 tags

    int len = strlen(line);
    char cpy[len];
    char tag[5];
    //char dummy2 = '\0';
    //tag = &dummy2;
    //char dummy3 = '\0';
    char *val; 
    val = cpy;
    
    char *data;
    int tagIndex = 0;
    int spaceIndex= 0;
    strcpy(val, line);
    bool whiteSpace = 0;
    int lengthOfTag=0;
    bool valExist=0;
    if(val[0] != '\t' && val[0] != ' '){
        spaceIndex++;
    }
                                             
    for(int i=0; i<len;i++){
        if(val[i] == '\t' || val[i] == ' '){
            if(whiteSpace){
                continue;
            }
            else{
                spaceIndex++;
                whiteSpace = 1;
            }
        }
        else{
            if(whiteSpace){
                if(spaceIndex == 2){
                    lengthOfTag++;
                }
                whiteSpace = 0;
            }
            else{
                if(spaceIndex ==2){
                    lengthOfTag++;
                    continue;
                }
            }
        }
    }

   
   for(int i = 0; i<len; i++){
       char c = val[i];
        if(c != ' '&& c!= '\t' && c!= '\n'){
           if(isdigit(c) && tagIndex == 0){
               continue;
           }
           if(tagIndex<lengthOfTag){
               tag[tagIndex] = c;
               tagIndex++;
               if(tagIndex == lengthOfTag){
                   tag[tagIndex] = '\0';
                   tagIndex++;
                   continue;
               }
               continue;
           }
       }
       if(tagIndex == lengthOfTag+1 && c !=' ' ){
            data = &val[i];
            valExist = 1 ;
            tagIndex++;
       }
       else if(c == '\n'){
           val[i] = '\0';
            
       }
                                                     
    }
    tag[tagIndex] = '\0';
    bool tagFound = 0;
    for(int i=0; i<101; i++){
        if(strcmp(tag,tags[i])==0){
            tagFound = 1;
        }
    }
    

    Field *newField = malloc(sizeof(Field));
    if(tagFound){
    ///// make sure everything is valid
        //printf("%d\n", (int) strlen(data));
        newField->tag= malloc((sizeof(char)*(strlen(tag)+1)));
        if(newField->tag == NULL){
            free(newField);
            return NULL;
        }
        if(!valExist){
            //strcpy(data, " ");
            data = " ";
        }
        newField ->value = malloc(sizeof(char)*(1+strlen(data)));
        if(newField->value == NULL){
            free(newField);
            free(newField->tag);
            return NULL;
        }
        strcpy(newField->value, data);
        //newField->value = newValue;

        strcpy(newField->tag, tag);
        //newField->tag = newTag;
    }
    else{
        free(newField);
        return NULL;
    }
    printf("%s, %s\n", newField->tag, newField->value);
    return newField;
}

Field* createSubmitterField(char* line, int level){

    char * lv1tags[] = {"NAME", "ADDR", "OBJE", "LANG", "RFN", "RIN", 
                            "NOTE", "CHAN", "PHON", "EMAIL", "FAX",
                             "WWW"};//12
    char * lv2tags[] = {"CONT", "ADR1", "ADR2", "ADR3", "CITY", 
                            "STAE", "POST", "CTRY", "FILE", 
                            "TITL", "CONC", "DATE","NOTE"};//13
    char * lv3tags[] = {"FORM", "CONT", "CONC", "TIME"};//4

    int len = strlen(line);
    char tag[5];
    char val[len];
    char *data;
    int tagIndex = 0;
    int spaceIndex= 0;
    strcpy(val, line);
    bool whiteSpace = 0;
    int lengthOfTag=0;
    if(val[0] != '\t' && val[0] != ' '){
        spaceIndex++;
    }

    for(int i=0; i<len;i++){
        if(val[i] == '\t' || val[i] == ' '){
            if(whiteSpace){
                continue;
            }
            else{
                spaceIndex++;
                whiteSpace = 1;
            }
        }
        else{
            if(whiteSpace){
                if(spaceIndex == 2){
                    lengthOfTag++;
                }
                whiteSpace = 0;
            }
            else{
                if(spaceIndex ==2){
                    lengthOfTag++;
                    continue;
                }
            }
        }
    }
 
    for(int i = 0; i<len; i++){
        char c = val[i];
        if(c != ' '&& c!= '\t' && c!= '\n'){
            if(isdigit(c) && tagIndex == 0){
                continue;
            }
            if(tagIndex<lengthOfTag){
                tag[tagIndex] = c;
                tagIndex++;
                if(tagIndex == lengthOfTag){
                    tag[tagIndex] = '\0';
                    tagIndex++;
                    continue;
                }
                continue;
            }
        }
        if(tagIndex == lengthOfTag+1 && c !=' ' ){
             data = &val[i];
             tagIndex++;
        }
        else if(c == '\n'){
            val[i] = '\0';
        }

    }
    bool tagFound = 0;
    if(level == 1){
        for(int i=0; i<12; i++){
            if(strcmp(lv1tags[i],tag)==0){
                tagFound = 1;
            }
        }
    }
    else if(level == 2){
        for(int i=0; i<9; i++){
            if(strcmp(lv2tags[i],tag)==0){
                //store in linked list
                tagFound = 1;
            }
        }
    }
    else if(level == 3){
        for(int i=0; i<4; i++){
            if(strcmp(lv3tags[i],tag)==0){
                //store in linked list
                tagFound = 1;
            }
        }
    }
    
    
    Field *newField = malloc(sizeof(Field));
    if(tagFound){
    ///// make sure everything is valid
        //printf("%d\n", (int) strlen(data));
        char * newTag = malloc(sizeof(char)*(strlen(tag)+1));
        if(newTag == NULL){
            free(newField);
            return NULL;
        }
        char * newValue = malloc(sizeof(char)*(strlen(data)+1));
        if(newValue == NULL){
            free(newField);
            free(newTag);
            return NULL;
        }
        strcpy(newTag, tag);
        strcpy(newValue, data);
        newField->tag = newTag;
        newField->value = newValue;
    }
    else{
        free(newField);
        return NULL;
    }
    return newField;
}

Field* createHeaderField(char* line, int level){
    char * lv1tags[] = {"SOUR", "DEST", "DATE", "SUBM", "SUBN", "FILE", "COPR",
                        "GEDC", "CHAR", "LANG", "PLAC", "NOTE"}; // 12 ITEMS
    char * lv2tags[] = {"VERS", "NAME", "CORP", "DATA", "TIME", "VERS", "FORM",
                        "CONC", "CONT"}; // 9 ITEMS
    char * lv3tags[] = {"ADDR", "DATE", "COPR", "CONT", "CONC"}; //4 ITEMS
    char * lv4tags[] = {"CONT", "CONC"}; //2 ITEMS

    int len = strlen(line);
    char tag[5];
    char val[len];
    char *data;
    char dummy = '\0';
    data = &dummy;
    int tagIndex = 0;
    strcpy(val, line);
    for(int i = 0; i<len; i++){
        char c = val[i];
        if(c != ' '&& c!= '\t' && c!= '\n'){
            if(isdigit(c) && tagIndex == 0){
                continue;
            }
            if(tagIndex<4){
                tag[tagIndex] = c;
                tagIndex++;
                if(tagIndex == 4){
                    tag[tagIndex] = '\0';
                    tagIndex++;
                    continue;
                }
                continue;
            }
        }
        if(tagIndex == 5 && c !=' ' ){

       /////         printf(">>%c\n", c);
             data = &val[i];
             tagIndex++;
        
        }
        else if(c == '\n'){
            val[i] = '\0';
        }

    }
    bool tagFound = 0;
    if(level == 1){
        for(int i=0; i<12; i++){
            if(strcmp(lv1tags[i],tag)==0){
                tagFound = 1;
            }
        }
    }
    else if(level == 2){
        for(int i=0; i<9; i++){
            if(strcmp(lv2tags[i],tag)==0){
                //store in linked list
                tagFound = 1;
            }
        }
    }
    else if(level == 3){
        for(int i=0; i<4; i++){
            if(strcmp(lv3tags[i],tag)==0){
                //store in linked list
                tagFound = 1;
            }
        }
    }
    else if(level == 4){
        for(int i=0; i<2; i++){
            if(strcmp(lv4tags[i], tag)==0){
                //store in linked list
                tagFound = 1;
            }
        }
    }
    
    
    Field *newField = malloc(sizeof(Field));
    if(tagFound){
    ///// make sure everything is valid
        int len = strlen(data);
        if(len == 0){
            char emptydata[] = "<empty>";
            data = emptydata;
        }



//        printf(">>>%s %d\n", data, (int) strlen(data));
        char * newTag = malloc(sizeof(char)*(strlen(tag)+1));
        char * newValue = malloc(sizeof(char)*(strlen(data)+1));
        strcpy(newTag, tag);
        strcpy(newValue, data);
        newField->tag = newTag;
        newField->value = newValue;

    }
    else{
        return NULL;
    }

    return newField;
}

int findCurrentLevel(char* line){
    //returns -1 if there is an error
    for(int i = 0; i< strlen(line); i++){
        if(line[i] != ' ' && line[i] != '\t'){
            int levelCheck;
            char level[4];
            int j = i;
            
            //printf(">>>>>>%d", j);
            while(line[j] != '\0'){
                if(j >=  i+3){
                    return -1;
                }
                if(line[j] == ' '){
                    level[j-i] = '\0';
                    break;
                }
                else if(isdigit(line[j])){
                    level[j-i] = line[j];
                }
                else{
                    return -1;
                }
                j++;
            }
            levelCheck = atoi(level);
            return levelCheck;   
        }
    }
 return -1;
}

RecordType findRecordType(char* line){

    int len = strlen(line);
    bool tagOpen = 0;
    bool tagClosed = 0;
    bool record = 0;
    char dummy = '\0';
    char * type = &dummy;
    int typeIndex = 0;

    for(int i = 0; i<len;i++){
        //find first open @ tag
        if(!tagOpen){
            if(line[i] == '@'){
                tagOpen = 1;
                continue;
            }
        }
        else if(tagOpen && !tagClosed){
            if(line[i] == '@'){
                tagClosed = 1;
                continue;
            }
       }
        else if(tagOpen && tagClosed && !record){
            if(line[i] != ' '){
                record = 1;
            }
        }

        if(record){
            if(line[i] == '\n' || line[i] == '\0'){
                type[typeIndex] = '\0';
                break;
            }
            else if(line[i] == ' '){continue;}
            else if(typeIndex >= 4){
                type[typeIndex] = '\0';
                break;
            }
            else{
                type[typeIndex] = line[i];
                typeIndex ++;
                continue;
            }
        }
    }
    if(type == NULL){
        return INVALID;
    }
    else{
        char types[typeIndex+1];
	    char * tp = type;
        tp[typeIndex] = '\0';
        strcpy(types, tp);
        if(strcmp(types, "INDI") == 0){return INDIVIDUAL;}
        else if(strcmp(types, "FAM") == 0){return FAMILY;}
        else if(strcmp(types, "SUBM") == 0){return SUBMITTER;}
        else{return INVALID;}
    }
}
        
void printIndividualList(List * indiList){
    ListIterator iter = createIterator(*indiList);
    Individual* indi;
    indi = nextElement(&iter);
    while (indi != NULL){
        printf("%s\n", indi->givenName);
        indi = nextElement(&iter);
    }

}






