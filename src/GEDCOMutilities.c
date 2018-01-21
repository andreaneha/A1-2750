#include "GEDCOMutilities.h"

Header* createHeader(char* source, float gedcVersion, 
CharSet encoding, Submitter* submitter, List otherFields){
    printf("createHeader\n");
    return NULL;
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
        
