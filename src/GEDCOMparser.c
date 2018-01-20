#include "GEDCOMparser.h"


GEDCOMerror createGEDCOM(char* fileName, GEDCOMobject** obj){
    //printf(">>>>>>%s\n", fileName);
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

    while(fgets(line,1000,file)!= NULL){
        // First Parse Header
        if(strcmp(line, "0 HEAD\n") == 0){
            if(headerExists == 0){
                // there has not been a header in the file yet. 
                // this is valid

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
        
        if(headerExists){
            printf("%s", line);


        }
        




 
 
 
 
 
 /*
 
        if (lineCounter == 0){
            if(strcmp(line, "0 HEAD\n") != 0){ //****flexible line terminators! keep in mind***
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
