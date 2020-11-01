//included pointers with 2d array, working as of 31/10 1134pm


#include <stdio.h>
#include <time.h> //library used for execution time

// function
int readfile();

// global variable declaration
#define trs1 90 //set training set first 90 patient
#define tes1 10 //set testing set last 10 patient
#define row 100 //set total row of dataset
#define col 10 //set total col of dataset

int main()
{
    clock_t tstart = clock(); //start clock
    readfile();
    printf("input3.c Time taken: %.2fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC); //print out execution time
    return 0;
}

int readfile()
{
    // initialise a1 and a2 to 0 for array
    int a1 = 0; //FOR loop usage check row
    int a2 = 0; //FOR loop usage check column
    float trainset[trs1][col]; //training array set 90,10
    float testset[tes1][col]; //testing array set 10,10
    float *ptrainset = &trainset[0][0]; //pointing to address starting from trainset[0][0]
    float *ptestset = &testset[0][0]; //pointing to address starting from trainset[0][0]
    FILE *mydata=fopen(".\\data_1-4.txt", "r"); //import dataset.txt file
    if (mydata == NULL)//file checker
    {
        printf("The file cannot be found. %s", mydata); 
    }
    else
    {
        //print out all the dataset float number excluding comma seperator and store into array
        for (a1 = 0; a1 < row; a1++) //check row not exceed 99 data count
        {
            for (a2 = 0; a2 < col; a2++) //check col not exceed 9 data count
            {
                //condition for first 90 training set
                if (a1 < trs1)
                {
                    fscanf(mydata, "%f, ", &trainset[a1][a2]); //get content from dataset.txt file and ignore comma and store in 2d array
                    printf("Training set patient: %d, Attributes Input: %d, pointer address: %p, array data: %0.2f \n", a1 + 1, a2 + 1, &*((ptrainset + a1*10) + a2), *((ptrainset + a1*10) + a2));
                }
                else
                {
                    fscanf(mydata, "%f, ", &testset[a1][a2]); //get content from dataset.txt file and ignore comma and store in 2d array
                    //condition for last 10 testing set
                    printf("Testing set patient: %d, Attributes Input: %d, pointer address: %p, array data: %0.2f \n", a1 + 1, a2 + 1, &*((ptestset + a1*10) + a2), *((ptestset + a1*10) + a2));
                }

            }
        }
    fclose(mydata); //close imported file and exit.
    }
}