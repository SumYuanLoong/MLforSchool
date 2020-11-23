#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <errno.h>

//error handling
extern int errno;

//static things
#define TMAE 0.25
#define trainspeed 0.05
#define TotalRows 100
#define TrRow 90 //number of rows in the training set
#define TsRow 10 //number of rows in the test set
#define col 10 //columns of data including desired value "result"

//datasets
float TrainSetData[TrRow][col-1]; //training data set
float TestSetData[TsRow][col-1]; //testing data set
float TrainSetDiag[TrRow]; //training result set 
float TestSetDiag[TsRow]; //testing result set

//pointers to the datasets
float *pTrainSetData = &TrainSetData[0][0];
float *pTestSetData = &TestSetData[0][0];
float *pTrainSetDiag = &TrainSetDiag[0];
float *pTestSetDiag = &TestSetDiag[0];

//functions in use
int readfile();

double random();

int main(/*consider cmd line args*/){
    clock_t tstart = clock(); //start clock
    int i;
    //everything in between
    readfile();
    int arraySize = sizeof(TrainSetData)/sizeof(*pTrainSetData);

    //DATA INPUT CHECKER
    // for(i=0; i< arraySize;i++){
    //     printf("%d . %f\n", i, *pTrainSetData);
    //     pTrainSetData++;
    // }



    printf("Time taken: %.2fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC); //print out execution time
    return 0;
}

int readfile(/*consider cmd line args*/){
    int x , y;
    FILE *fertfile_ptr = fopen("fertility_Diagnosis_Data_Group1_4.txt", "r");

    // error handling
    if (fertfile_ptr==NULL)
    {   
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        exit(1);
    }

    for(x=0; x<TotalRows; x++){
        for(y=0; y<col;y++){
            if(y==(col-1)){
                if(x<TrRow){
                    fscanf(fertfile_ptr, "%f, ",&TrainSetDiag[x]);
                }
                else{
                    fscanf(fertfile_ptr, "%f, ",&TestSetDiag[x-TrRow]);
                }
            }else{
                if(x<TrRow){
                    fscanf(fertfile_ptr, "%f, ",&TrainSetData[x][y]);
                }
                else{
                    fscanf(fertfile_ptr, "%f, ",&TestSetData[x-TrRow][y]);
                }
            }
        }
    }
    fclose(fertfile_ptr);
    return 0;
}

double random()
{
    int w;
    double resultrand; 
    w = (rand() % 3) - 1; //random between int -1, 0 , 1
    if(w > 1 || w < -1)
    {
        w = (rand() % 3) - 1; //random between int -1, 0 , 1
        //printf("%d", w);
    }
    if (w==0)
    {
        w=1;
    }
    //to improve the random result for double -1.00 to 1.00 by using w
    resultrand = (1.0*rand()/RAND_MAX - w);
    if(resultrand > 1.00)
    {
        resultrand = resultrand - 1;
    }
    //printf("\nweight = %lf", resultrand);
    return resultrand;
}