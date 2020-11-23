#include <stdio.h>
#include <stdlib.h>


void main(){
    //Creating some data
    double rndT[20];
    int x2,i;
    
    //write to a temp file
    FILE * temp = fopen("data.temp", "w");

    for(x2=0;x2<200;x2){
        
        double i = 1.0/++x2;
        //rndT[x2-1] = i;
        printf("%.5f\n", i);
        fprintf(temp, "%d %f\n", x2, i);
    }
    fclose(temp);
    
    //call GNUplot
    FILE * gnuplotPipe = _popen ("gnuplot -persist ", "w");

    //plotting
    for (i=0; i < 1; i++)
    {
    fprintf(gnuplotPipe, "%s \n", "plot 'data.temp'"); //Send commands to gnuplot one by one.
    }
    _pclose(gnuplotPipe);// process will stall here automatically no need to getch() anymore
    //however any printf under here will not print till gnuplot window is closed

    printf("Press any key to end\n");
    //getch(); //get char function to stall the exit of the program so the graph is displayed

    //fclose(gnuplotPipe);
}
