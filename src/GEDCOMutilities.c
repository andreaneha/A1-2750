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
        