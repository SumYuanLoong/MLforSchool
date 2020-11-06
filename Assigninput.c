#include <stdio.h>
#include <stdlib.h>

int main()
{
    double allfertdata[100][10];
    double traininpdata[90][9];
    double trainoutpdata[90][1];
    double testinpdata[10][9];
    double testoutpdata[10][1];
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
        /*Used to Check if data was successfully populated from the text file into the Two Dimensional Array
        printf("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", allfertdata[a][0], allfertdata[a][1], allfertdata[a][2], allfertdata[a][3], allfertdata[a][4], allfertdata[a][5], allfertdata[a][6], allfertdata[a][7], allfertdata[a][8], allfertdata[a][9]);
        Remove comment to check*/
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
                }

                else if (sc == 9)
                {
                    trainoutpdata[sr][0] = allfertdata[sr][sc];
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
            printf("%lf ", traininpdata[test1][test2]);
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
            printf("%lf", trainoutpdata[test1][test2]);
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
}
