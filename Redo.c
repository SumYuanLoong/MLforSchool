#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//static things
#define TMAE 0.25
#define trainspeed 0.05
#define totalRows 100
#define trRow 90 //number of rows in the training set
#define tsRow 10 //number of rows in the test set
#define col 10 //columns of data including desired value "result"

//datasets
float TrainSetData[trRow][col-1]; //training data set
float TestSetData[tsRow][col-1]; //testing data set
float TrainSetDiag[trRow]; //training result set 
float TestSetDiag[tsRow]; //testing result set
double trainz[trRow]; //store training set sigma summation of each patient
double testz[tsRow]; //store testing set sigma summation of each patient
double trainsig[trRow]; //store training set sigmoid y cap of each patient
double testsig[tsRow]; //store testing set sigmoid y cap of each patient

//pointers to the datasets
float *pTrainSetData = &TrainSetData[0][0];
float *pTestSetData = &TestSetData[0][0];
float *pTrainSetDiag = &TrainSetDiag[0];
float *pTestSetDiag = &TestSetDiag[0];
double *ptrainz = &trainz[0];
double *ptestz = &testz[0];

//original data/weights/bias for printing at end
double originalWeight[9];
double originalBias; //change to 9
double utrmmse, utsmmse;
double *putrmmse = &utrmmse;
double *putsmmse = &utsmmse;

//functions in use
void readFile();
/* flag 1 for training, 0 for test*/
void linearRegress(short flag);
void sigmoid(double zArr[], double sigArr[]);
void mmseFunc(double *trainMmse,double *testMmse);
double random();

int main(/*consider cmd line args*/){
    clock_t tstart = clock(); //start clock
    //everything in between
    readFile();

    //TESTING CODE
    //int arraySize = sizeof(TrainSetData)/sizeof(*pTrainSetData);

    //DATA INPUT CHECKER
    // for(i=0; i< arraySize;i++){
    //     printf("%d . %f\n", i, *pTrainSetData);
    //     pTrainSetData++;
    // }

    //CREATION OF ORIGINAL WEIGHTS/BIAS
    int i;
    originalBias = 1;
    //printf("originalBias: %lf\n", originalBias);
    for (i = 0; i <= (col-1); i++)
    {
        originalWeight[i] = 1;
        //printf("originalWeight[%d]:%lf\n", i, originalWeight[i]);
    }

    //COLLECTION OF ORIGINAL DATA RUN1
    linearRegress(1);
    linearRegress(0);
    sigmoid(trainz,trainsig);
    sigmoid(testz,testsig);
    mmseFunc(putrmmse, putsmmse);
    printf("2utrmmse %lf , utsmmse %lf", utrmmse,utsmmse);
    //COLLECTION OF INITIAL DATA COMPLETE





    printf("Time taken: %.2fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC); //print out execution time
    return 0;
}

void readFile(){
    int x , y;
    FILE *fertfile_ptr = fopen("fertility_Diagnosis_Data_Group1_4.txt", "r");

    // error handling
    if (fertfile_ptr==NULL)
    {   
        fprintf(stderr, "Error opening file: ");
        exit(EXIT_FAILURE);
    }

    for(x=0; x<totalRows; x++){
        for(y=0; y<col;y++){
            if(y==(col-1)){ //result of diagnosis
                if(x<trRow){
                    fscanf(fertfile_ptr, "%f, ",&TrainSetDiag[x]);
                }
                else{
                    fscanf(fertfile_ptr, "%f, ",&TestSetDiag[x-trRow]);
                }
            }else{  //data to determine diagnosis
                if(x<trRow){
                    fscanf(fertfile_ptr, "%f, ",&TrainSetData[x][y]);
                }
                else{
                    fscanf(fertfile_ptr, "%f, ",&TestSetData[x-trRow][y]);
                }
            }
        }
    }
    fclose(fertfile_ptr);
}

void linearRegress(short flag){
    //TO check if run through of entire dataset is needed
    int maxRows;
    float *pdataset;
    double *pzArr;
    //assignment of pointers by choice
    if(flag == 1){
        maxRows = sizeof(TrainSetData)/sizeof(*pTrainSetData);
        pdataset = pTrainSetData;     
        pzArr = ptrainz;
    }
    else{
        maxRows = sizeof(TestSetData)/sizeof(*pTestSetData);
        pdataset = pTestSetData;    
        pzArr = ptestz;    
    }

    int a , b,c = 0; // x is loop counter, y for position in row, c for row
    double z = 0;
    for(a=0, b=0; a< maxRows; a++,pdataset++){
        if(b == (col-2)){
            z += (originalWeight[b] * *pdataset) + originalBias;
            *pzArr = z;
            pzArr++;   // increment to next value in z arrary
            b=0;
            z=0;        //reset column and z row values
        }
        else
        {
            z += (originalWeight[b++] * *pdataset);     //calculation
        }
    }
}

void sigmoid(double zArr[], double sigArr[]){
    int i,arrSz = sizeof(*zArr)/sizeof(zArr[0]);
    for(i=0;i<arrSz;i++){
        sigArr[i] = (1/(1+exp(-zArr[i])));
    }
}

void mmseFunc(double *trainMmse,double *testMmse){
    int i =0;
    double mmsesum = 0;

    for(i=0;i<trRow;i++){
        mmsesum += (pow((trainsig[i] - TrainSetDiag[i]),2));
    }
    *trainMmse = mmsesum/trRow;
    mmsesum =0;
    for(i=0;i<tsRow;i++){
        mmsesum += (pow((testsig[i] - TestSetDiag[i]),2));
    }
    *testMmse = mmsesum/tsRow;
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
        w=1;
    //to improve the random result for double -1.00 to 1.00 by using w
    resultrand = (1.0*rand()/RAND_MAX - w);
    if(resultrand > 1.00)
    {
        resultrand = resultrand - 1;
    }
    //printf("\nweight = %lf", resultrand);
    return resultrand;
}