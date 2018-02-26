#include "LinkedListAPI.h"
#include "GEDCOMparser.h"
#include "GEDCOMutilities.h"

/*
list of testsuits
1 - GEDCOMparse errors
2 - GEDCOMparse happy path
3 - create header


*/


int main(int argc, char **argv){

    GEDCOMerror gError;
    GEDCOMobject ** gObjectArray;
    GEDCOMobject * anObject;
    GEDCOMobject object;
    object.header = NULL;
    //(object.families) = NULL;
    //(object.individuals) = NULL;
    object.submitter = NULL;

    anObject = &object;
    gObjectArray = &anObject;
    int error;
    int desiredTestSuit = 3;
    if(desiredTestSuit == 3){
        //field = createHeaderField("1 SOUR FTW\n", 1);
        //printf(">>%s:%s\n", field->tag, field->value);

       // printf("Check how the parse file function handels things\n");
       gError = createGEDCOM(argv[1], gObjectArray);
       anObject = *gObjectArray;
       printf("Done\n");



  /*      printf("\nCheck the createSubmitterField function\n");
        Field * field;
        field = createSubmitterField("   1   NAME\n", 1);
        printf("%s:%s\n", field->tag, field->value);

*/
  //      Field * field1;
  //      field1 = createIndiField("2 ADDR\n", 2);

    //    printf("%s:%s\n", field1->tag, field1->value);



        //printf("\nCreate individual field\n");


        
    }





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
