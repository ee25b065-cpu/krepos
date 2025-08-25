/*EE25B065 , EE25B066 , EE25B145*/

/*Take as command line input four arguments: mean1, mean2, stdev and N
Use the srand() and rand() to generate 2 arrays of N uniformly distributed random numbers [either use malloc() and free(), or check that N is smaller than your array size]
Use the Box-Muller transformation to convert it into 2 arrays of numbers in a normal distribution
Shift the distributions so they have their averages at mean1 and mean2
Run the Student t- test (independent 2 sample t-test) and print the t-value
Create a plot of t versus N 
Useful links
https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
https://en.wikipedia.org/wiki/Student%27s_t-test
https://www.scribbr.com/statistics/students-t-table/
https://webspace.ship.edu/pgmarr/Geo441/Examples/Minimum%20Detectable%20Difference.pdf*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void boxmuller(float arr[],int N,float arrf[]);
void transform(float m,float arr[],int N,float std);
void studentttest(int N,float arr1[],float arr2[]);
float calcmean(float arr[],int N);
float calcvar(float arr[],int N,float mean);



void boxmuller(float arr[],int N,float arrf[]){
    int i=0;
    while(i<N){
        if (i+1 < N) {
            float u1 = arr[i];
            float u2 = arr[i+1];
            
            float factor = sqrt(-2.0f * log(u1));
            float z1 = (factor * cos(2.0f * M_PI * u2));
            float z2 = (factor * sin(2.0f * M_PI * u2));
            
            arrf[i] = z1;
            arrf[i+1] = z2;
            i += 2;
        } else {
            float u1 = arr[i];
            float u2;
            do {
                u2 = (float)rand() / RAND_MAX;
            } while (u2 == 0.0f || u2 == 1.0f);
            
            float factor = sqrt(-2.0f * log(u1));
            float z1 = (factor * cos(2.0f * M_PI * u2));
            
            arrf[i] = z1;
            i++;
        }
    }
}   

void transform(float m,float arr[],int N,float std){
    float tempmean;
    float s;
    
    tempmean=calcmean(arr,N);
    s=sqrt(calcvar(arr,N,tempmean));
    for(int i=0;i<N;i++){
        arr[i]=((arr[i]-tempmean)*std/s)+m;
    }
}


void studentttest(int N,float arr1[],float arr2[]){
   float t;
   float mean1;
   float mean2;
   float var1;
   float var2;
   float s;

   mean1=calcmean(arr1,N);
   mean2=calcmean(arr2,N);  
   var1=calcvar(arr1,N,mean1);
   var2=calcvar(arr2,N,mean2);
   
   s=sqrt((var1+var2)/2.0f * (2.0f/N));

   t=(mean1-mean2)/s;      
        
   printf("%f\n",t);
}

float calcmean(float arr[],int N){
    float mean=0.0f;
    for (int i=0;i<N;i++){
        mean+=arr[i];
    }
    mean/=N;
    return mean;
}

float calcvar(float arr[],int N,float mean){
    float var=0.0f;
    for(int i=0;i<N;i++){
        float dif=arr[i]-mean;
        var+=dif*dif;
    }
    var/=(N-1);
    return var;
}



int main(int argc, char *argv[]){
    float m1;
    float m2;
    int N;
    float stdev;   
    if (argc != 5) {
        printf("Usage: %s <mean1> <mean2> <stddev> <sample_size>\n", argv[0]);
        return 1;
    }
    srand(time(NULL));

    m1 = atof(argv[1]);
    m2 = atof(argv[2]);
    N = atoi(argv[4]);
    stdev = atof(argv[3]);

    if (N <= 1) {
        printf("Error: Sample size must be greater than 1\n");
        return 1;
    }
    if (stdev <= 0.0f) {
        printf("Error: Standard deviation must be positive\n");
        return 1;
    }

    float *arr1=malloc(N*sizeof(float));
    float *arr2=malloc(N*sizeof(float));
    float *arr1f=malloc(N*sizeof(float));
    float *arr2f=malloc(N*sizeof(float));

    if (!arr1 || !arr2 || !arr1f || !arr2f) {
        printf("Error: Memory allocation failed\n");
        free(arr1); free(arr2); free(arr1f); free(arr2f);
        return 1;
    }

    /*Filling the two arrays with random uniform values in the range (0,1)*/
    for (int i=0;i<N;i++){
        do {
            arr1[i] = (float)rand() / RAND_MAX;
            arr2[i] = (float)rand() / RAND_MAX;
        } while (arr1[i] == 0.0f || arr1[i] == 1.0f || arr2[i] == 0.0f || arr2[i] == 1.0f);
        
        
    }
    
    /*Performing Box-Muller transformation on the two arrays*/
    boxmuller(arr1,N,arr1f);
    boxmuller(arr2,N,arr2f);
    
    /*Shifting the means of the distributions to required means and scaling them according to the given standard deviation*/
    transform(m1,arr1f,N,stdev);
    transform(m2,arr2f,N,stdev);
    
    /*Running Student's independent two sample t-test*/
    studentttest(N,arr1f,arr2f);

    free(arr1);
    free(arr2);
    free(arr1f);
    free(arr2f);
    return 0;
}