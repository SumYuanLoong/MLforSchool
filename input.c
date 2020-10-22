#include <stdio.h>

// function
int readfile();

// global variable declaration
const int trs1 = 90; //set training set first 90 patient
const int tes1 = 10; //set testing set last 10 patient
const int row = 100; //set total row of dataset
const int col = 10; //set total col of dataset

int main()
{
    readfile();
    return 0;
}

int readfile()
{
    // initialise a1 and a2 to 0 for array
    int a1 = 0;
    int a2 = 0;
    float mydatanumber; //naming for dataset float number
    float trainset[trs1][col]; //training array set
    float testset[tes1][col]; //testing array set
    FILE *mydata=fopen(".\\data_1-4.txt", "r"); //import dataset.txt file
    if (mydata == NULL)//file checker
    {
        printf("The file cannot be found. %s", mydata); 
    }
    else
    {
        //print out all the dataset float number excluding comma seperator and store into array
        for (a1 = 0; a1 < row; a1++) //check row not exceed 100 data count
        {
            for (a2 = 0; a2 < col; a2++) //check col not exceed 10 data count
            {
                fscanf(mydata, "%f, ", &mydatanumber); //get content from dataset.txt file and ignore comma
                //condition for first 90 training set
                if (a1 < trs1)
                {
                    trainset[a1][a2] = mydatanumber; //store dataset float number into training set array
                    printf("Training set patient: %d, Attributes Input: %d, array data: %.2f \n", a1 + 1, a2 + 1, trainset[a1][a2]);
                }
                else
                {
                    //condition for last 10 testing set
                    testset[a1][a2] = mydatanumber; //store dataset float number into testing set array
                    printf("Testing set patient: %d, Attributes Input: %d, array data: %.2f \n", a1 + 1, a2 + 1, testset[a1][a2]);
                }
            }
        }
    fclose(mydata); //close imported file and exit.
    }
}