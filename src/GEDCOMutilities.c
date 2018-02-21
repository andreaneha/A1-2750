#include "GEDCOMutilities.h"


void * printEventFields(List * list){
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
    Field * field = NULL;
    printf("%s :%d\n", line, currentLevel);

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
         char * newTag = malloc(sizeof(char)*strlen(tag));
         if(!valExist){
             //strcpy(data, " ");
             data = " ";
         }
         char * newValue = malloc(sizeof(char)*strlen(data));
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
     
    char *fieldTag = field->tag;
    char *fieldVal = field->value;
    bool eventFound = 0;

    for(int i; i<23;i++){
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
    List * ret = NULL;

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
        id = malloc((sizeof(char))*letterCount);
        strcpy(id, at);
        ref = malloc(sizeof(Ref));
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

Header * createHeader(List* headerFieldList){

	ListIterator iter = createIterator(*headerFieldList);
    Field * field;    

    field = (Field*)nextElement(&iter);
    
    Header * header;
    header = malloc(sizeof(Header));

    bool delField ;

    while(field != NULL){
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
            //submitter pointer goes here
            header->submitter = NULL;
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
    for(int i=0; i<101; i++){
        if(strcmp(tag,tags[i])==0){
            tagFound = 1;
        }
    }

    Field *newField = malloc(sizeof(Field));
    if(tagFound){
    ///// make sure everything is valid
        //printf("%d\n", (int) strlen(data));
        char * newTag = malloc(sizeof(char)*strlen(tag));
        if(!valExist){
            //strcpy(data, " ");
            data = " ";
        }
        char * newValue = malloc(sizeof(char)*strlen(data));
        strcpy(newValue, data);
        newField->value = newValue;

        strcpy(newTag, tag);
        newField->tag = newTag;
    }
    else{
        return NULL;
    }
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
        char * newTag = malloc(sizeof(char)*strlen(tag));
        char * newValue = malloc(sizeof(char)*strlen(data));
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
    //    printf("%d\n", (int) strlen(data));
        char * newTag = malloc(sizeof(char)*strlen(tag));
        char * newValue = malloc(sizeof(char)*strlen(data));
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
    char type [5];
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

    if(strcmp(type, "INDI") == 0){return INDIVIDUAL;}
    else if(strcmp(type, "FAM") == 0){return FAMILY;}
    else if(strcmp(type, "SUBM") == 0){return SUBMITTER;}
    else{return INVALID;}
}
        
