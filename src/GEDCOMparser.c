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
    recordType currentType=0;
    int currentLevel=0;

    while(fgets(line,1000,file)!= NULL){
        // First Parse Header
        if(strcmp(line, "0 HEAD\n") == 0){
            if(!headerExists){
                // there has not been a header in the file yet. 
                // this is valid
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
            //keep track of the current level
                levelCheck = findCurrentLevel(line);

                    //printf("%d\n", levelCheck);


                    if(levelCheck == currentLevel+1){
                        currentLevel ++;
                    }
                    else if(levelCheck < currentLevel && levelCheck != 0){
                        currentLevel = levelCheck;
                    } 
                    else if(levelCheck == 0){
                        //figure out way to check the type of record

                        currentType = 1;
                        currentLevel = levelCheck;

                    }
                    else{
                        g.type = 2;
                        g.line = lineCounter;

                    }


                    if(currentType == 0){
                        printf("%c\n", line[i]);
                    }
                }
                else{
                    break;
                }
                
            }

        }


        




 
 
 
 
 
 /*
 
        if (lineCounter == 0){
            if(strcmp(line, "0 HEAD\n") != 0){ // ****flexible line terminators! keep in mind***
                   g.type = 2;
                   g.line = -1;
                   return g;
            }
            else{
                headerExists=1;
                printf("header Exists");
                if
           }
       }
*/







        lineCounter++;

    }






    g.type = 0;
    g.line = 1; 
    return g;
}
