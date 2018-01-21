#include "LinkedListAPI.h"
#include "GEDCOMparser.h"
#include "GEDCOMutilities.h"


int main(int argc, char **argv){

    GEDCOMerror gError;
    GEDCOMobject ** gObjectArray;
    GEDCOMobject * anObject;
    GEDCOMobject object;
    object.header = NULL;
    object.families = initializeList(NULL, NULL,NULL);
    object.individuals = initializeList(NULL, NULL, NULL);
    object.submitter = NULL;

    anObject = &object;
    gObjectArray = &anObject;
    int error;
    int desiredTestSuit = 2;

/*

Testing create GEDCOM function. Testcases:
- invalid file with correct extension
- invalid extension
- no extension
- null string
- null GEDCOMobj

*/

    if(desiredTestSuit == 2){
        
        gError = createGEDCOM(argv[1], gObjectArray);

    }




    if(desiredTestSuit==1){

        printf("Testing createGEDCOM\n");
        printf("--------------------\n");
        error = 0;
        
        gError =  createGEDCOM("andrea.ged", gObjectArray);
        if(gError.type == 1 && gError.line == -1){
            error ++;
//            printf(">>%d (should be 1)\n", error);
        }
        gError =  createGEDCOM("andrea.something.else", gObjectArray);       
        if(gError.type == 1 && gError.line == -1){
            error ++;
//            printf(">>%d (should be 2)\n", error);
        }
        gError =  createGEDCOM("andrea", gObjectArray);
        if(gError.type == 1 && gError.line == -1){
            error ++;
//            printf(">>%d (should be 3)\n", error);
        }
        gError =  createGEDCOM(NULL, gObjectArray);
        if(gError.type == 1 && gError.line == -1){
            error ++;
//            printf(">>%d (should be 4)\n", error);
        }
        gError =  createGEDCOM("test", gObjectArray);
        if(gError.type == 1 && gError.line == -1){
            error ++;
//            printf(">>%d (should be 5)\n", error);
        }

        //if(gError.type == 2 && gError.line == -1){
          //  error ++; 
//            printf(">>%d (should be 6)\n", error);
        //}
        if(error == 5){
            printf("createGEDCOM error checks work\n");
        }

        //ierror = 0;








    }

return 0;
}
