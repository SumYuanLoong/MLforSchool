#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*----Define static variable----*/
#define TMAE 0.15
#define trainspeed 0.05
#define totalRows 100
#define trRow 90 //number of rows in the training set
#define tsRow 10 //number of rows in the test set
#define col 10 //columns of data including desired value "result"
#define neurons 7

/*----Arrays to store dataset----*/
float TrainSetData[trRow][col-1]; //training data set
float TestSetData[tsRow][col-1]; //testing data set
float TrainSetDiag[trRow]; //training result set 
float TestSetDiag[tsRow]; //testing result set

float trainz[neurons][trRow]; //store training set z value of each patient
float testz[neurons][tsRow]; //store testing set z value of each patient
float trainsig[neurons][trRow]; //store training set sigmoid y cap of each patient
float testsig[neurons][tsRow]; //store testing set sigmoid y cap of each patient
float neuTrainZ[trRow],neuTrainSig[trRow]; //Z value and y cap value for the output layer neuron
float neuTestZ[tsRow],neuTestSig[tsRow]; //Z value and y cap value for the output layer neuron
float oldCentralW[neurons], errorByCentralN; //store old output layer weight and error

/*----Pointers of the datasets----*/
float *pTrainSetData = &TrainSetData[0][0]; //pointers for training data set
float *pTestSetData = &TestSetData[0][0]; //pointers for testing data set
float *pTrainSetDiag = &TrainSetDiag[0]; //pointers for training data set output desire(d) value
float *pTestSetDiag = &TestSetDiag[0]; //pointers for testing data set output desire(d) value
float *ptrainz = &trainz[0][0]; //pointers for hidden layer training data set z
float *ptestz = &testz[0][0]; //pointers for hidden layer training data set z
float *pneuTrainZ = &neuTrainZ[0]; //pointers for output layer training data set z
float *pneuTestZ = &neuTestZ[0]; //pointers for output layer training data set z

/*----Original data/weights/bias for printing----*/
float weight[neurons][9]; //weights for the hidden layer neurons
float bias[neurons]; //bias for the hidden layer neurons
float neuWeight[neurons]; //weights for the output neuron combining the neurons
float neuBias; //bias for the output layer neurons
double utrmmse, utsmmse, ttrmmse, ttsmmse; //untrain and trained MMSE for both training and testing data set
double *putrmmse = &utrmmse; //pointers for untrain training data set MMSE
double *putsmmse = &utsmmse; //pointers for untrain testing data set MMSE
double *pttrmmse = &ttrmmse; //pointers for train training data set MMSE
double *pttsmmse = &ttsmmse; //pointers for train testing data set MMSE

/*----List of functions in use----*/
void readFile();
/* flag 1 for training, 0 for testing data set*/
void linearRegress(short flag);
void neuronRegress(short flag);
float sigmoid(double z);
void mmseFunc(double *trainMmse,double *testMmse);
double maeFunc();
void neuBackPropagate();
void backPropagate();
double random();
void matrix();
void patientNew();

//Start of program
int main(){
    clock_t tstart = clock(); //start clock
    srand(time(NULL));
    readFile();

    //Perform initialization of random between -1 to 1 weight and bias
    int x,y;
    neuBias = random();
    printf("Output Layer Bias: %lf \n", neuBias);
    for(x =0;x<neurons;x++){
        neuWeight[x] = random();
        printf("Weight for Neuron[%d] in Output Layer: %lf\n",x+1,neuWeight[x]);
        bias[x] = random();
        printf("Neuron[%d], Initial Bias: %lf\n", x+1,bias[x]);
        for (y = 0; y < col-1; y++)
        {
            weight[x][y] =random();
            printf("Neuron[%d], Initial Weight[%d]: %lf\n", x+1, y+1, weight[x][y]);
        }
    }

    //Collection of original data RUN1 (hidden layer)
    linearRegress(1);
    linearRegress(0);
    neuronRegress(1);
    neuronRegress(0);
    //Collection of original untrained MMSE for both train and test set
    mmseFunc(putrmmse, putsmmse);
    //printf("utrmmse = %f, utsmmse = %f",utrmmse,utsmmse);
    //COLLECTION OF INITIAL DATA COMPLETE

    //Recording of mae and interation data into multi.temp file and used for generating GNUplot
    FILE * temp = fopen("multi.temp", "w");
    int iteration = 1;
    double mae = maeFunc();
    
    //Validating 
    while (mae > TMAE)
    {
        fprintf(temp, "%d %f\n", iteration, mae);
        neuBackPropagate();
        linearRegress(1);
        neuronRegress(1);
        iteration++;
        mae = maeFunc();
        //printf("mae = %f\n",mae);
    }
    fclose(temp);

    //Perform calculation using trained weight and bias for testing set
    linearRegress(0);
    neuronRegress(0);
    mmseFunc(pttrmmse,pttsmmse);

    //Printing output
    printf("\nTotal Iteration: %d\n", iteration);
    printf("Trained MAE (%lf) <= %lf \n", maeFunc(), TMAE);
    printf("Training Set: Untrained MMSE = %lf\tTrained MMSE = %lf\n", *putrmmse, *pttrmmse);
    printf("Testing Set:  Untrained MMSE = %lf\tTrained MMSE = %lf\n", *putsmmse, *pttsmmse);

    //Compute and print confusion matrix
    matrix();

    //Print execution time
    printf("Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC); //print out execution time
    printf("Close GNUPLOT for input of new Patient");
    
    printf("Final output Layer Bias: %lf \n", neuBias);
    for(x=0;x<neurons;x++){
        printf("Final weight for Neuron[%d] in Output Layer: %lf\n",x+1,neuWeight[x]);
        printf("Neuron[%d], Final Bias: %lf\n", x+1,bias[x]);
        for(y=0;y<col-2;y++){
            
            printf("Neuron[%d], Final Weight[%d]: %lf\n", x+1, y+1, weight[x][y]);
        }
    }

    //Perform GNUplot
    FILE * gnuplotPipe = _popen ("gnuplot -persist ", "w");
    fprintf(gnuplotPipe, "%s \n", "plot 'multi.temp' with line");
    _pclose(gnuplotPipe);

    patientNew();
    return 0;
}

//Function to read data set file using file pointer and store in array
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

//Function to compute hidden Layer linear regression
void linearRegress(short flag){
    //To check if run through of entire dataset is needed
    int maxRows;
    float *pdataset, *reset, *pzArr, *pSigArr;

    //Assignment of pointers by choice
    //Choice flag == 1, is for training data set
    if(flag == 1){
        maxRows = sizeof(TrainSetData)/sizeof(*pTrainSetData);
        pdataset = pTrainSetData;     
        pzArr = ptrainz;
        pSigArr = &trainsig[0][0];
    }
    //Choice flag == 0, is for testing data set
    else if(flag == 0){
        maxRows = sizeof(TestSetData)/sizeof(*pTestSetData);
        pdataset = pTestSetData;    
        pzArr = ptestz;    
        pSigArr = &testsig[0][0];
    }
    reset = pdataset;

    //Compute linear regression, flag=1 training dataset / flag = 0 testing data set
    int n, a , b = 0; // b is loop counter, a for position in row, n is for neuron
    double z = 0;
    for(n=0;n<neurons;n++){
        for (a = 0, b = 0; a < maxRows; a++, pdataset++)
        { //inside of 1 neuron
            if (b == (col - 2))
            {
                z += (weight[n][b] * *pdataset) + bias[n];
                *pzArr = z;
                *pSigArr = sigmoid(z);
                pzArr++; // increment to next value in z arrary
                pSigArr++;
                b = 0;
                z = 0; //reset column and z row values
            }
            else
            {
                z += (weight[n][b++] * *pdataset); //calculation for z
            }
        }
        pdataset = reset;
        //reset pdataset
    }
}

//Function to compute output layer linear regression
void neuronRegress(short flag){
    int maxRows;
    float *pdataset, *pzArr,*pSigArr;

    //Choice flag == 1, is for training data set
    if(flag == 1){
        maxRows = sizeof(trainsig)/sizeof(trainsig[0][0]);
        pdataset = &trainsig[0][0];     
        pzArr = pneuTrainZ;
        pSigArr = &neuTrainSig[0];
    }
    //Choice flag == 0, is for testing data set
    else{
        maxRows = sizeof(testsig)/sizeof(testsig[0][0]);
        pdataset = &testsig[0][0];    
        pzArr = pneuTestZ;    
        pSigArr = &neuTestSig[0];
    }

    //Compute linear regression, flag=1 training dataset / flag = 0 testing data set
    int a,b,c = 0; // b is loop counter, a for position in row
    double z = 0;
    for(a=0, b=0; a< maxRows; a++,pdataset++){
        if(b == (neurons-1)){
            z += (neuWeight[b] * *pdataset) + neuBias;
            *pzArr = z;
            *pSigArr = sigmoid(z);
            pSigArr++;
            pzArr++;   // increment to next value in z arrary
            b=0;
            z=0;        //reset column and z row values
        }
        else
        {
            z += (neuWeight[b++] * *pdataset);     //calculation for z
        }
    }
}

//Function to compute sigmoid calculation
float sigmoid(double z){
    return (1/(1+exp(-z)));
}

//Function to compute MMSE    
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

//Function to compute MAE, abs - absolute value(non-negative)
double maeFunc(){
    int i;
    double maesum = 0;
    for(i=0; i<trRow;i++){
        maesum += fabs(neuTrainSig[i] - TrainSetDiag[i]);
    }
    return maesum/90;
}

//Function to compute output layer backward propagation
void neuBackPropagate()
{
    int x, y; //x for the rows of data y for which output layer neuron the data is coming from
    double sumtrainw = 0, sumtrainb = 0;
    for (y = 0; y < neurons; y++)
    {
        for (x = 0; x < trRow; x++)
        {
            sumtrainw += (neuTrainSig[x] - TrainSetDiag[x]) * (exp(neuTrainZ[x]) / ((1 + exp(neuTrainZ[x])) * (1 + exp(neuTrainZ[x])))) * trainsig[y][x];
            if (y == neurons-1)
            {
                sumtrainb += (neuTrainSig[x] - TrainSetDiag[x]) * (exp(neuTrainZ[x]) / ((1 + exp(neuTrainZ[x])) * (1 + exp(neuTrainZ[x]))));
            }
        }
        sumtrainw = (sumtrainw / trRow); 
        //Saved and reused in hidden layer old weight is also saved and reused
        oldCentralW[y] = neuWeight[y]; //saving the old weight to be used in the back propagation for the hidden layer neurons
        neuWeight[y] = (neuWeight[y] - (trainspeed * sumtrainw)); //update the new weight into oldw[0-8]
        sumtrainw = 0;
    }
    sumtrainb = (sumtrainb / trRow);
    errorByCentralN = sumtrainb;    //saving the error in the output layer neuron
    neuBias = (neuBias - (trainspeed * sumtrainb)); //update the new bias b into oldb
    backPropagate();
}

//Function to compute hidden layer backward propagation
void backPropagate(){
    int x, y, n; //x for rows of data, y for which column of data, n for which hidden layer neuron
    double sumtrainw = 0, sumtrainb = 0, outErrNWeight;
    for (n = 0; n < neurons; n++)
    {
        outErrNWeight = errorByCentralN * oldCentralW[n];
        for (y = 0; y < col-1; y++)
        {
            for (x = 0; x < trRow; x++)
            {
                sumtrainw += outErrNWeight * (exp(trainz[n][x]) / ((1 + exp(trainz[n][x])) * (1 + exp(trainz[n][x])))) * TrainSetData[x][y];
                if (y == col-2){
                    sumtrainb += outErrNWeight * (exp(trainz[n][x]) / ((1 + exp(trainz[n][x])) * (1 + exp(trainz[n][x]))));
                }
            }
            sumtrainw = (sumtrainw / trRow);
            weight[n][y] = weight[n][y] - (trainspeed * sumtrainw); //update the new weight
            sumtrainw =0;
        }
        sumtrainb = sumtrainb/trRow;
        bias[n] = bias[n] - (trainspeed*sumtrainb); //update the new bias
        sumtrainb =0;
    }
}

//Function to compute and print confusion matrix
void matrix(){
    int tp=0, fp=0, tn=0, fn=0, i, y;
    for(i=0;i<trRow;i++){
        y = round(neuTrainSig[i]);
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
    printf("\n-------------------------------------------\n\n");
    printf("Training Set Confusion Matrix\n                          True      False\n");
    printf("Predicted Positive        %d         %d\n",tp,fp);
    printf("Predicted Negative        %d        %d\n",tn,fn);
    printf("\n-------------------------------------------\n\n");

    tp=0, fp=0, tn=0, fn=0;

    for(i=0;i<tsRow;i++){
        y=round(neuTestSig[i]);    
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
    printf("Testing Set Confusion Matrix\n                          True      False\n");
    printf("Predicted Positive        %d         %d\n",tp,fp);
    printf("Predicted Negative        %d         %d",tn,fn);
    printf("\n\n-------------------------------------------\n\n");
}

//Function to generate random number between -1 to 1
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

void patientNew(){
    int i, n;
    float x[9], xSig[3], result;
    double z=0;
    printf("Please key in the 9 attributes\n");
    printf("Please key in the Season: ");
    scanf("%f", &x[0]);
    printf("You keyed in %f \n",x[0]);
    printf("Please key in the Age: ");
    scanf("%f", &x[1]);
    printf("You keyed in %f \n",x[1]);
    printf("Please key in the Childish Disease ");
    scanf("%f", &x[2]);
    printf("You keyed in %f \n",x[2]);
    printf("Please key in Accident: ");
    scanf("%f", &x[3]);
    printf("You keyed in %f \n",x[3]);
    printf("Please key in Surgical Intervention: ");
    scanf("%f", &x[4]);
    printf("You keyed in %f \n",x[4]);
    printf("Please key in High fevers in last year: ");
    scanf("%f", &x[5]);
    printf("You keyed in %f \n",x[5]);
    printf("Please key in frequency of alcohol: ");
    scanf("%f", &x[6]);
    printf("You keyed in %f \n",x[6]);
    printf("Please key in smoking habit: ");
    scanf("%f", &x[7]);
    printf("You keyed in %f \n",x[7]);
    printf("Please key in hours sitting per day: ");
    scanf("%f", &x[8]);
    printf("You keyed in %f \n",x[8]);


    //copy of linearRegress()
    for(n=0;n<neurons;n++){
        for(i=0;i<(col-1);i++){
            if (i == (col - 2))
            {
                z += (weight[n][i] * x[i]) + bias[n];
                xSig[n] = sigmoid(z);
                z = 0; //reset z row values
            }
            else
            {
                z += (weight[n][i] * x[i]); //calculation for z
            }
        }
    }

    //copy of neuronRegress()
    for(n=0;n<neurons;n++){
        
            if (n == (neurons - 1))
            {
                z += (neuWeight[n] * xSig[n]) + neuBias;
                result = round(sigmoid(z));
                z = 0; //reset z row values
            }
            else
            {
                z += (neuWeight[n] * xSig[i]); //calculation for z
            }
        
    }
    if(result == 0)
    printf("The Predicted Result is: Negative");
    else
    {
        printf("The Predicted Result is: Positive");
    }
}