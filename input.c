#include <stdio.h>

float readfile();

int main()
{
    readfile();
    return 0;
}

float readfile()
{
    float mydatanumber; //naming for dataset float number
    const int row = 100;
    const int col = 10;
    float arraydata[row][col]; //array for dataset float number
    int a1 = 0;
    int a2 = 0;
    FILE *mydata=fopen(".\\data_1-4.txt", "r"); //import dataset.txt file
    
    if (mydata == NULL)//file checker
    {
        printf("The file cannot be found. %s", mydata); 
    }
    else
    {
        //print out all the dataset float number excluding comma seperator and store into array
        for (a1 = 0; a1 < row; a1++)
        {
            for (a2 = 0; a2 < col; a2++)
            {
                fscanf(mydata, "%f, ", &mydatanumber); //get content from dataset.txt file and ignore comma
                arraydata[a1][a2] = mydatanumber; //store dataset float number into array
                //printing output for easy reference, can be removed in future.
                printf("Patient: %d, Data input: %d, array data: %.2f \n", a1 + 1, a2 + 1, arraydata[a1][a2]);
            }
        }
    fclose(mydata); //close imported file and exit.
    }
}