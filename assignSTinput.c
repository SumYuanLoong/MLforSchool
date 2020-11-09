#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TMAE 0.25
#define trainspeed 0.05
#define trow 89 //for train set row
#define tcol 9 //for train set col
#define tsrow 9 //for test set row
#define tscol 9 //for test set col

//functions
double process();
double sigsumz();
double mmsefunc();
double maefunc();
double random();

//global variable
double allfertdata[100][10];
double traininpdata[90][9]; //store untrain trainning set input
double trainoutpdata[90][1]; //store untrain trainning set d
double testinpdata[10][9]; //store untrain testing set input
double testoutpdata[10][1]; //store untrain testing set d

double trainz[trow]; //store training set sigma summation of each patient
double testz[tsrow]; //store testing set sigma summation of each patient
double trainsig[trow]; //store training set sigmoid y cap of each patient
double testsig[tsrow]; //store testing set sigmoid y cap of each patient
double utmmse; //store untrain mmse training set
double utsmmse; //store untrain mmse testing set
int ummsecheck = 0; //check if ummse stored
double ttmmse; //trained mmse training set
double ttsmmse; //trained mmse testing set
double mae; //store mae
double oldw[9]; //change to 9 because 1 set only got 9 different weight and it will be same throughout different sets
double trainedw[9]; //to update trained input data
double oldb; //change to 9
double trainedb[9]; //to update trained output data called bias

int main()
{
    //declaration and initialize for process() function usage
    // initializes once for generate random number
    time_t t;   
    srand(time(NULL)); //Intializes random number generator

    /*Lecture 10 Slide 5: Creating a Pointer Variable that points to a structure of type FILE*/
    FILE *fertfile_ptr;
    
    /*Lecture 10 Slide 9: Assign the created Pointer Variable to point to and open the Fertility Diagnosis Data text file*/
    fertfile_ptr = fopen("fertility_Diagnosis_Data_Group1_4.txt", "r");
    
    /*Lecture 10 Slide 10: Checking for error when opening file, return error msg if error encountered*/
    
    if ((fertfile_ptr = fopen("fertility_Diagnosis_Data_Group1_4.txt", "r"))==NULL)
    {
        printf("Input file could not be opened \n");
        exit(1);
    }
    
    int a;
    /*Lecture 10 Slide 40: Using FOR Loop to read each row and column of data in the Fertility Diagnosis Data text file and putting the data into a Two Dimensional Array*/
    for(a=0; a<=99; a++)
    {
        fscanf(fertfile_ptr, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf", &allfertdata[a][0], &allfertdata[a][1], &allfertdata[a][2], &allfertdata[a][3], &allfertdata[a][4], &allfertdata[a][5], &allfertdata[a][6], &allfertdata[a][7], &allfertdata[a][8], &allfertdata[a][9]);
        //Used to Check if data was successfully populated from the text file into the Two Dimensional Array
        //printf("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", allfertdata[a][0], allfertdata[a][1], allfertdata[a][2], allfertdata[a][3], allfertdata[a][4], allfertdata[a][5], allfertdata[a][6], allfertdata[a][7], allfertdata[a][8], allfertdata[a][9]);
        //Remove comment to check
    }

    //Close the opened file
    fclose(fertfile_ptr);

    int sr;
    int sc;
    int nc=0;
    /*Categorizing all the data in the Two Dimensional Array into specific categories by putting them into 4 different Two Dimensional Arrays*/
    for(sr=0; sr<=99; sr++)
    {
        if(sr <= 89)
        {
            for(sc=0; sc<=9; sc++)
            {
                if(sc <= 8)
                {
                    traininpdata[sr][sc] = allfertdata[sr][sc];
                    //printf("\ntraininpdata[%d][%d]:%lf", sr, sc, allfertdata[sr][sc]);
                }

                else if (sc == 9)
                {
                    trainoutpdata[sr][0] = allfertdata[sr][sc];
                    //printf("\ntrainoutpdata[%d][%d]:%lf", sr, 0, allfertdata[sr][sc]);
                }
                
            }
        }

        else if (sr > 89 && sr <= 99)
        {
            for(sc=0; sc<=9; sc++)
            {
                if(sc <= 8)
                {
                    testinpdata[nc][sc] = allfertdata[sr][sc];
                    //printf("\ntestinput[%d][%d] = %lf ", nc, sc, testinpdata[nc][sc]);
                }

                else if (sc == 9)
                {
                    testoutpdata[nc][0] = allfertdata[sr][sc];
                }
            }
            nc += 1;
        }
    }

    /*Check traininpdata
    int test1;
    int test2;
    for(test1=0; test1<=89; test1++)
    {
        for(test2=0; test2<=8; test2++)
        {
            printf("\ntraininpdata[%d][%d]:%lf ", test1, test2, traininpdata[test1][test2]);
        }
        printf("\n");
    }*/

    /*Check trainoutpdata
    int test1;
    int test2;
    for(test1=0; test1<=89; test1++)
    {
        for(test2=0; test2<=0; test2++)
        {
            printf("trainoutpdata[%d][%d]:%lf", test1,test2,trainoutpdata[test1][test2]);
        }
        printf("\n");
    }*/

    /*Check testinpdata
    int test1;
    int test2;
    for(test1=0; test1<=9; test1++)
    {
        for(test2=0; test2<=8; test2++)
        {
            printf("%lf ", testinpdata[test1][test2]);
        }
        printf("\n");
    }
    */

    /*Check testoutpdata
    int test1;
    int test2;
    for(test1=0; test1<=9; test1++)
    {
        for(test2=0; test2<=0; test2++)
        {
            printf("%lf", testoutpdata[test1][test2]);
        }
        printf("\n");
    }
    */

    process();
}

double process()
{
    int a, b, c;
    double sumtrainw = 0;
    for (b = 0; b <= 9; b++)
    {
        oldw[b] = 1;
        oldb = 1;
    }
    sigsumz();
    mmsefunc();
    maefunc();

    /*
    for(a = 0; a <= 89; a++)
    {
    printf("\nsummation z[%d] = %f", a, trainz[a]);
    printf("\nsigmoid y[%d] = %f d[%d] = %f", a, trainsig[a], a, trainoutpdata[a][0]);
    }
    printf("\nuntrained mmse (1*(summation ycap - d)^2)/90 = %f", ummse);
    printf("\nmae (1*(summation ycap - d))/90 = %f\n", mae);
    */

    while(mae > TMAE)
    {
        printf("\nmae (1*(summation ycap - d))/90 = %f\n", mae);
        for(a = 0; a <= 8; a++)
        {
            //printf("\nsummation z[%d] = %f", a, trainz[a]);
            //printf("\nsigmoid y[%d] = %f d[%d] = %f", a, trainsig[a], a, trainoutpdata[a][0]);
            //printf("\nuntrained mmse (1*(summation ycap - d)^2)/90 = %f", ummse);
            //printf("\nmae (1*(summation ycap - d))/90 = %f\n", mae);

            for (b = 0; b <= 89; b++)
            {
                sumtrainw += ( (trainsig[b] - trainoutpdata[b][0]) * ( exp(trainz[b]) / ( 1 + exp(trainz[b]) )) * traininpdata[b][a]);
                //printf("\nsumtrainw[%d][%d] = %f", a, b, sumtrainw);
                //printf("\n%f", traininpdata[b][a]);
            }
            trainedw[a] = ( (1 * (sumtrainw)) / 90);
            oldw[a] = (oldw[a] - (trainspeed*trainedw[a]));
            trainedb[a] = ( (1 * (sumtrainw)) / 90);
            oldb = (oldb - (trainspeed*trainedb[a]));
            //printf("\ntrainedw[%d][%d] = %f", a, b, oldw[b]);
            sumtrainw = 0;
        }
        sigsumz();
        maefunc();
    }

    {
        for (a = 0; a <= 89; a++)
        {
            //printf("\nsummation z[%d] = %f", a, trainz[a]);
            //printf("\nsigmoid y[%d] = %f d[%d] = %f", a, trainsig[a], a, trainoutpdata[a][0]);
            //printf("\nuntrained mmse (1*(summation ycap - d)^2)/90 = %f", ummse);
            //printf("\nmae (1*(summation ycap - d))/90 = %f\n", mae);
        }
    }
}

//calculate z = summation of w1 * x1 + w2 * x2 ..... + b then calulate sigmoid y cap
double sigsumz()
{
    int a = 0, b = 0, c = 0;
    //initw = random(); //initialization of weight set w to random float number -1 to 1 replaced with trainedw
    //initb = random(); //initialization of bias set b to random float number -1 to 1
    //testing with 1 to confirm calculation, by default must use random number between float -1 to 1 for initial phase
    double sumz = 0; // + b
    double z = 0;
    //perform for training set first 90
    for(a=0; a <= 99; a++)
    {
        if(a <= trow)
        {
            for (b=0; b < tcol; b++)
            {
                if (b < 8)
                {
                    sumz += (oldw[b] * traininpdata[a][b]); //summation of z = w1*x1 + w2*x2.....
                    //printf("\n%d,%d traininputdata = %f \tsumz = %f", a, b, traininpdata[a][b], sumz);
                }
                else
                {
                    sumz += ((oldw[b]*traininpdata[a][b])+oldb); //summation of z = w1*x1 + w2*x2.....
                    trainz[a] = sumz;
                    //printf("\n%d,%d traininputdata = %f \ttrainz = %f", a, b, traininpdata[a][b], trainz[a]);
                    trainsig[a] = (1/(1+exp(-trainz[a]))); //calculate sigmoid function = y^ = ycap.
                    //printf("\n trainsigmoid y cap[%d] = %f \t trainoutpdata d[%d] = %f", a, trainsig[a], a, trainoutpdata[a][0]);
                }
            }
            sumz = 0; //reset sumz to default 0 + b to count for next patient
        }
        //perform for testing set last 10 set
        else if(a > 89 && a<=99)
        {
            for(b=0; b < tscol; b++)
            {
                if(b < 8)
                {
                    sumz += (oldw[b] * testinpdata[c][b]); //summation of z = w1*x1 + w2*x2.....
                    //printf("\n%d,%d testinpdata[%d][%d] = %f \tsumz = %f", a, b, c, b, testinpdata[c][b], sumz);
                }
                else
                {
                    sumz += ((oldw[b] * testinpdata[c][b]) + oldb); //summation of z = w1*x1 + w2*x2.....
                    testz[c] = sumz;
                    //printf("\n%d,%d testinpdata[%d][%d] = %f \ttestz = %f", a, b, c, b, testinpdata[c][b], testz[c]);
                    testsig[c] = (1/(1+exp(-testz[c]))); //calculate sigmoid function = y^ = ycap.
                    //printf("\n testsigmoid y cap[%d] = %f \t testoutpdata d[%d][0] = %f", a, testsig[c], c, testoutpdata[c][0]);
                }
            }
            c += 1; //to retrieve value from correct testinpdata[c][b]array
            sumz = 0; //reset sumz to default 0 + b to count for next patient
        }
    }
}

//perform mmse function mmse = 1/90 * summation (y^ - d)^2
double mmsefunc()
{
    int a, b, c = 0;
    double tmmsesum = 0, tsmmsesum = 0;
    
    for(a=0; a<= 89; a++)
    {
        tmmsesum += (pow((trainsig[a] - trainoutpdata[a][0]),2));
        //printf("\n mmsesum + (ycap(%f) - d(%f), pow 2) = mmsesum[%d][0](%f)", trainsig[a], trainoutpdata[a][0], a, tmmsesum);
        if(a <= 9)
        {
            tsmmsesum += (pow((testsig[a] - testoutpdata[a][0]),2));
            //printf("\n mmsesum + (ycap(%f) - d(%f), pow 2) = mmsesum[%d][0](%f)", testsig[a], testoutpdata[a][0], a, tsmmsesum);
        }
    }
    if(ummsecheck == 0)
    {
        utmmse = (1*tmmsesum)/90; //untrain mmse training set
        utsmmse = (1*tsmmsesum)/90; //untrain mmse testing set
        //printf("\n untrained train mmse[%d] = %f \n untrained test mmse[%d] = %f", a, utmmse, a, utsmmse);
        ummsecheck = 1;
    }
    else
    {
        ttmmse = (1*tmmsesum)/90; //untrain mmse training set
        ttsmmse = (1*tsmmsesum)/90; //untrain mmse testing set
        //printf("\n trained tmmse[%d] = %f", a, ttmmse);            
    }
    tmmsesum = 0;
    tsmmsesum = 0;
}

double maefunc()
{
    int a,b;
    double maesum = 0;
    for(a=0; a <= 89; a++)
    {
        maesum += fabs(trainsig[a] - trainoutpdata[a][0]);
        //printf("\nmaesum + ycap(%f) - d(%f) = maesum %f", trainsig[a], trainoutpdata[a][0], maesum);   
    }
    mae = (1*maesum)/90; //fabs = float absolute
    //printf("\n mae[%d] = %f", a, mae);
    maesum = 0;
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
    return(resultrand);
}