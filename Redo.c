#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//errors to bring up whereever trow, tcol 89 9 are used

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
double weight[9];
double bias;
double utrmmse, utsmmse, ttrmmse, ttsmmse;
double *putrmmse = &utrmmse;
double *putsmmse = &utsmmse;
double *pttrmmse = &ttrmmse;
double *pttsmmse = &ttsmmse;

//functions in use
void readFile();
/* flag 1 for training, 0 for test*/
void linearRegress(short flag);
void sigmoid(double zArr[], double sigArr[],int arrSz);
void mmseFunc(double *trainMmse,double *testMmse);
double maeFunc();
void backPropagate();
double random();
void matrix();

int main(/*consider cmd line args*/){
    clock_t tstart = clock(); //start clock
    srand(time(NULL));
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
    bias = random();
    printf("bias: %lf\n", bias);
    for (i = 0; i <= (col-1); i++)
    {
        weight[i] = random();
        printf("weight[%d]:%lf\n", i, weight[i]);
    }

    //COLLECTION OF ORIGINAL DATA RUN1
    linearRegress(1);
    linearRegress(0);
    sigmoid(trainz,trainsig,trRow);
    sigmoid(testz,testsig,tsRow);
    mmseFunc(putrmmse, putsmmse);
    //printf("2utrmmse %lf , utsmmse %lf", utrmmse,utsmmse);
    //COLLECTION OF INITIAL DATA COMPLETE

    FILE * temp = fopen("single.temp", "w");

    int iteration = 1;
    double mae = maeFunc();
    while (mae > TMAE){
        fprintf(temp, "%d %f\n", iteration, mae);
        backPropagate();
        linearRegress(1);
        sigmoid(trainz,trainsig,trRow);
        iteration++;
        mae = maeFunc();
    }
    fclose(temp);


    //learning complete do forward once for testing set
    linearRegress(0);
    sigmoid(testz,testsig,tsRow);
    mmseFunc(pttrmmse,pttsmmse);

    //printing output
    printf("total iteration:%d\n", iteration);
    printf("trained mae(%lf) <= %lf \n", maeFunc(), TMAE);
    printf("training set:untrained mmse = %lf\ttrained mmse = %lf\n", *putrmmse, *pttrmmse);
    printf("testing set:untrained mmse = %lf\ttrained mmse = %lf\n", *putsmmse, *pttsmmse);

    matrix();
    
    
    printf("Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC); //print out execution time

    FILE * gnuplotPipe = _popen ("gnuplot -persist ", "w");
    fprintf(gnuplotPipe, "%s \n", "plot 'single.temp' with line");
    _pclose(gnuplotPipe);
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
            z += (weight[b] * *pdataset) + bias;
            *pzArr = z;
            pzArr++;   // increment to next value in z arrary
            b=0;
            z=0;        //reset column and z row values
        }
        else
        {
            z += (weight[b++] * *pdataset);     //calculation
        }
    }
}

void sigmoid(double zArr[], double sigArr[],int arrSz){
    int i;
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

double maeFunc(){
    int i;
    double maesum = 0;
    for(i=0; i<trRow;i++){
        maesum += fabs(trainsig[i] - TrainSetDiag[i]);
    }
    return maesum/90;
}

void backPropagate(){
    int x,y;
    double sumtrainw = 0, sumtrainb = 0;
    for (y = 0; y < col-1; y++)
    {
        //printf("\nsummation z[%d] = %f", a, trainz[a]);
        //printf("\nsigmoid y[%d] = %f d[%d] = %f", a, trainsig[a], a, trainoutpdata[a][0]);
        //printf("\nuntrained mmse (1*(summation ycap - d)^2)/90 = %f", ummse);
        //printf("\nmae (1*(summation ycap - d))/90 = %f\n", mae);

        for (x = 0; x < trRow; x++)
        {
            sumtrainw += (trainsig[x] - TrainSetDiag[x]) * (exp(trainz[x]) / ((1 + exp(trainz[x]))*(1 + exp(trainz[x]))) * TrainSetData[x][y]);
            //printf("\nsumtrainw[%d][%d] = %f", a, b, sumtrainw);
            if (y == 8)
            {
                sumtrainb += (trainsig[x] - TrainSetDiag[x]) * (exp(trainz[x]) / ((1 + exp(trainz[x]))*(1 + exp(trainz[x]))) * 1);
                //printf("\nsumtrainb[%d][%d] = %f", x, y, sumtrainb);

            }
        }
        sumtrainw = (sumtrainw / trRow);
        weight[y] = (weight[y] - (trainspeed * sumtrainw)); //update the new weight into oldw[0-8]
        //printf("\ntrainedw[%d][%d] = %f", x, y, weight[y]);
        sumtrainb = (sumtrainb / trRow);
        bias = (bias - (trainspeed * sumtrainb)); //update the new bias b into oldb
        sumtrainw = 0;
        sumtrainb = 0;
    }
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

void matrix(){
    int tp =0, fp=0, tn=0, fn=0, i, y;
    short res [totalRows];
    for(i=0;i<trRow;i++){
        y = round(trainsig[i]);
        if (y == 1)
        {
            if (TrainSetDiag[i] == y)
                tp++;
            else
                fp++;
        }
        else
        {
            if (TrainSetDiag[i] == y)
                tn++;
            else
                fn++;
        }
    }
    printf("Training set confusion matrix\n                true    false\n");
    printf("predicted true     %d    %d\n",tp,fp);
    printf("predicted false    %d    %d\n",tn,fn);
    tp =0, fp=0, tn=0, fn=0;

    for(i=0;i<tsRow;i++){
        y=round(testsig[i]);    
        if (y == 1)
        {
            if (TestSetDiag[i] == y)
                tp++;
            else
                fp++;
        }
        else
        {
            if (TestSetDiag[i] == y)
                tn++;
            else
                fn++;
        }
    }
    printf("testing set confusion matrix\n                true    false\n");
    printf("predicted true     %d    %d\n",tp,fp);
    printf("predicted false    %d    %d\n",tn,fn);
}