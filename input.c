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
    float arraydata[1000]; //array for dataset float number
    int i = 0;
    FILE *mydata=fopen(".\\data_1-4.txt", "r"); //import dataset.txt file
    
    //print out all the dataset float number excluding comma seperator and store into array
    for (i == 0; i <= 1000; i++)
    {
        fscanf(mydata, "%f, ", &mydatanumber); //get content from dataset.txt file
        arraydata[i] = mydatanumber; //store dataset float number into array
        
        //printing output for easy reference, can be removed in future.
        printf("array data: %.2f", arraydata[i]);
        printf("\narray data count: %d:\t", i);
        printf("%.2f \n", mydatanumber);
    }
    
    fclose(mydata); //close imported file and exit.
}