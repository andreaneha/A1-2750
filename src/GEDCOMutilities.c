#include "GEDCOMutilities.h"

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
        if((c>='a' && c<='z') || (c>='A' && c<='Z')){
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
        
