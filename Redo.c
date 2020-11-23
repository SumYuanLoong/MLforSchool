#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//static things
#define TMAE 0.25
#define trainspeed 0.05
#define trow 90 //for train set row
#define tcol 9 //for train set col
#define tsrow 9 //for test set row
#define tscol 9 //for test set col

//global variables
float TrainSetData[90][9]; //training array set 90,10
float TestSetData[10][9]; //testing array set 10,10
float TrainSetDiag[90][1]; //training array set 90,10
float TestSetDiag[10][1]; //testing array set 10,10

//functions in use
int readfile();

int main(/*consider cmd line args*/){
    clock_t tstart = clock(); //start clock
    //everything in between





    printf("input3.c Time taken: %.2fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC); //print out execution time
    return 0;
}

int readfile(/*consider cmd line args*/){
    FILE *fertfile_ptr = fopen("fertility_Diagnosis_Data_Group1_4.txt", "r");

    if ((fertfile_ptr = fopen("fertility_Diagnosis_Data_Group1_4.txt", "r"))==NULL)
    {
        fprintf(stderr,"Input file could not be opened \n");
        exit(1);
    }

    return 0;
}