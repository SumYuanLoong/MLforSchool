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
#define neurons 3

//datasets
float TrainSetData[trRow][col-1]; //training data set
float TestSetData[tsRow][col-1]; //testing data set
float TrainSetDiag[trRow]; //training result set 
float TestSetDiag[tsRow]; //testing result set

float trainz[neurons][trRow]; //store training set z value of each patient
float testz[neurons][tsRow]; //store testing set z value of each patient
//float trainsig[trRow]; //store training set sigmoid y cap of each patient
//float testsig[tsRow]; //store testing set sigmoid y cap of each patient
float neuTrainZ[trRow],neuTrainSig[trRow]; //Z value and y cap value for the central neuron
float neuTestZ[tsRow],neuTestSig[tsRow]; //Z value and y cap value for the central neuron

//pointers to the datasets
float *pTrainSetData = &TrainSetData[0][0];
float *pTestSetData = &TestSetData[0][0];
float *pTrainSetDiag = &TrainSetDiag[0];
float *pTestSetDiag = &TestSetDiag[0];
float *ptrainz = &trainz[0][0];
float *ptestz = &testz[0][0];
float *pneuTrainZ = &neuTrainZ[0];
float *pneuTestZ = &neuTestZ[0];

//original data/weights/bias for printing at end
float weight[neurons][9]; //weights and bias for the hidden layer neurons
float bias[neurons];
float neuWeight[neurons]; //weights and bias for the central neuron combining the neurons
float neuBias;
double utrmmse, utsmmse, ttrmmse, ttsmmse;
double *putrmmse = &utrmmse;
double *putsmmse = &utsmmse;
double *pttrmmse = &ttrmmse;
double *pttsmmse = &ttsmmse;

//functions in use
void readFile();                //remain unchanged
/* flag 1 for training, 0 for test*/
void linearRegress(short flag);
void neuronRegress(short flag);
void sigmoid(double inArr[], double outArr[],int arrSz);
void mmseFunc(double *trainMmse,double *testMmse);
double maeFunc();
void backPropagate();
double random();
void matrix();

int main(){
    clock_t tstart = clock(); //start clock
    readFile();

    int x,y;
    neuBias = 1;
    for(x =0;x<neurons;x++){
        neuWeight[x] = 1;
        bias[x] = 1;
        printf("bias: %lf\n", bias[x]);
        for (y = 0; y < col; y++)
        {
            weight[x][y] = 1;
            printf("weight[%d]:%lf\n", y, weight[x][y]);
        }
    }
    linearRegress(1);
    linearRegress(0);
    neuronRegress(1);
    neuronRegress(0);
    sigmoid(neuTrainZ,neuTrainSig,trRow);
    sigmoid(neuTestZ,neuTestSig,tsRow);
    mmseFunc(putrmmse, putsmmse);
    printf("utrmmse = %f, utsmmse = %f",utrmmse,utsmmse);

    printf("test");
    

    return 0;
}

void linearRegress(short flag){
    //TO check if run through of entire dataset is needed
    int maxRows;
    float *pdataset, *reset;
    float *pzArr;
    //assignment of pointers by choice
    if(flag == 1){
        maxRows = sizeof(TrainSetData)/sizeof(*pTrainSetData);
        pdataset = pTrainSetData;     
        pzArr = ptrainz;
    }
    else if(flag == 0){
        maxRows = sizeof(TestSetData)/sizeof(*pTestSetData);
        pdataset = pTestSetData;    
        pzArr = ptestz;    
    }
    reset = pdataset;

    int n,a , b,c = 0; // x is loop counter, y for position in row, c for row, n is for neuron
    double z = 0;
    for(n=0;n<neurons;n++){
        for (a = 0, b = 0; a < maxRows; a++, pdataset++)
        { //inside of 1 neuron
            if (b == (col - 2))
            {
                z += (weight[n][b] * *pdataset) + bias[n];
                *pzArr = z;
                pzArr++; // increment to next value in z arrary
                b = 0;
                z = 0; //reset column and z row values
            }
            else
            {
                z += (weight[n][b++] * *pdataset); //calculation
            }
        }
        pdataset = reset;
        //reset pdataset
    }
}

void neuronRegress(short flag){
    int maxRows;
    float *pdataset;
    float *pzArr;
    if(flag == 1){
        maxRows = sizeof(trainz)/sizeof(*ptrainz);
        pdataset = ptrainz;     
        pzArr = pneuTrainZ;
    }
    else{
        maxRows = sizeof(testz)/sizeof(*ptestz);
        pdataset = ptestz;    
        pzArr = pneuTestZ;    
    }

    int a,b,c = 0; // x is loop counter, y for position in row, c for row
    double z = 0;
    for(a=0, b=0; a< maxRows; a++,pdataset++){
        if(b == (neurons-1)){
            z += (neuWeight[b] * *pdataset) + neuBias;
            //printf("nuez%d : %f\n",a,z);
            *pzArr = z;
            pzArr++;   // increment to next value in z arrary
            b=0;
            z=0;        //reset column and z row values
        }
        else
        {
            z += (neuWeight[b++] * *pdataset);     //calculation
        }
    }
}

void sigmoid(double inArr[], double outArr[],int arrSz){
    int i;
    for(i=0;i<arrSz;i++){
        outArr[i] = (1/(1+exp(-inArr[i])));
        printf("");
    }
}

void mmseFunc(double *trainMmse,double *testMmse){
    int i =0;
    double mmsesum = 0;

    for(i=0;i<trRow;i++){
        mmsesum += (pow((neuTrainSig[i] - TrainSetDiag[i]),2));
    }
    *trainMmse = mmsesum/trRow;
    mmsesum =0;
    for(i=0;i<tsRow;i++){
        mmsesum += (pow((neuTestSig[i] - TestSetDiag[i]),2));
    }
    *testMmse = mmsesum/tsRow;
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