#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void plotdata(int n);

int main(int argc,char *argv[]){
    srand(time(NULL));
    if (argc != 5) {
        printf("Usage: %s <r(min)> <r(max)> <r(step)> <number of iterations>\n", argv[0]);
        return 1;
    }
    float rmin=atof(argv[1]);
    float rmax=atof(argv[2]);
    float rstep=atof(argv[3]);
    int n_iter=atoi(argv[4]);
    if (rmin<0 || rmax<0 || n_iter<0){
        printf("Enter positive and sufficient values.\n");
        return 1;
    }
    FILE *fptr = fopen("logistic.txt", "w");
    if (fptr == NULL) {
    printf("Error: Could not create logistic.txt\n");
    return 1;
    }
    for(float r=rmin;r<=rmax;r+=rstep){
        float x2;
        float x1=(float)rand()/RAND_MAX;
        for (int n=1;n<=n_iter;n++){
            x2=r*x1*(1-x1);            
            fprintf(fptr,"%.6f\t%d\t%.6f\n",r,n,x2);
            x1=x2;
        }
    }
    fclose(fptr);
    plotdata(n_iter);
    return 0;
    
}

void plotdata(int n){
    FILE *gnupipe=popen("gnuplot -persistent","w");
    if (gnupipe==NULL){
        printf("Error: Could not open gnuplot. Make sure gnuplot is installed.\n");
        return;
    }
    fprintf(gnupipe, "set terminal jpeg size 800,600 font 'Arial,12'\n");
    fprintf(gnupipe, "set output 'EE25B065_logistic.jpg'\n");
    fprintf(gnupipe,"set title 'Logistic Map'\n");
    fprintf(gnupipe,"set xlabel 'r'\n");
    fprintf(gnupipe,"set ylabel 'x[n]'\n");
    fprintf(gnupipe,"plot 'logistic.txt' skip 1000 every %d using 1:3 with dots\n",n);

    fprintf(gnupipe, "set terminal wxt\n"); 
    fprintf(gnupipe, "set output\n");        
    fprintf(gnupipe, "replot\n");            
    
    fflush(gnupipe);
    pclose(gnupipe);
    
    printf("JPEG plot saved as 'EE25B065_logistic.jpg'\n");
    printf("Interactive plot displayed.\n");


}




