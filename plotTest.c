#include <stdio.h>
#include <stdlib.h>


void main(){
    //Creating some data
    double rndT[20];
    int x2;

    for(x2=0;x2<20;x2){
        
        double i = 1.0/++x2;
        rndT[x2-1] = i;
        printf("\n%.10f", i);
    }

    /*TO USE FILE INSTALL GNUPLOT : https://sourceforge.net/projects/gnuplot/files/latest/download
    Install with all settings default less 'Add to PATH enviroment variable'
    Restart VS code after install*/

    //call GNUplot
    FILE * gnuplotPipe = _popen ("gnuplot -persist ", "w");

    //set GNUplot styling
    fprintf(gnuplotPipe, "set title 'MAE over iterations' \n");

    //plotting
    fprintf(gnuplotPipe, "plot '-' with line \n");
    for(x2=0;x2<20;x2++){
        //adding data to the GNUplot pipe
        fprintf(gnuplotPipe, "%d %f\n", x2, rndT[x2]);
    }
    fprintf(gnuplotPipe, "e\n");
    fflush(gnuplotPipe);

    printf("\nPress any key to end");
    getch(); //get char function to stall the exit of the program so the graph is displayed

    //fclose(gnuplotPipe);
}
