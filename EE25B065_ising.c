/*EE25B065,EE25B066,EE25B147*/
/*Code built with help of claude.ai*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

void randinitialize(int arr[], int L);
float Mavg(int arr[], int L);
void switch_matrix(int **arr1, int **arr2);
double metrohastings(int arr1[], int arr2[], double Temp, int L, int N, float J, float dM);

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    if (argc != 8) {
        printf("Usage: %s <L(square length)> <J> <T_start> <T_end> <T_increment> <N(iterations for given T)> <dM(Tolerance)>\n", argv[0]);
        return 1;
    }
    
    int L = atoi(argv[1]);
    float J = atof(argv[2]);
    double T_start = atof(argv[3]);
    double T_end = atof(argv[4]);
    double T_increment = atof(argv[5]);
    int N = atoi(argv[6]);
    float dM = atof(argv[7]);

    if (L <= 0 || T_start <= 0 || T_end <= 0 || T_increment <= 0 || T_start > T_end || N <= 0) {
        printf("Please enter relevant and valid values for the required program\n");
        return 1;
    }

    int *Mold = malloc(L * L * sizeof(int));
    int *Mnew = malloc(L * L * sizeof(int));

    if (Mold == NULL || Mnew == NULL) {
        printf("Memory allocation failed\n");
        free(Mold);
        free(Mnew);
        return 1;
    }

    randinitialize(Mold,L);
    
    FILE *fptr = fopen("Mavg_vs_T.txt", "w");
    if (fptr == NULL) {
        printf("Error in recording values, please try again\n");
        free(Mold);
        free(Mnew);
        return 1;
    }
    
    fprintf(fptr, "Temperature\tAbsMagnetization\n");
    
    for (double T = T_start; T <= T_end; T += T_increment) {
        
        memcpy(Mnew, Mold, L * L * sizeof(int));
        
        double M_abs = metrohastings(Mold, Mnew, T, L, N, J, dM);
        
        fprintf(fptr, "%.6lf\t\t%.6lf\n", T, M_abs);
        printf("T = %.3lf, |M| = %.6f\n", T, M_abs);
        
        
        memcpy(Mold, Mnew, L * L * sizeof(int));
    }
    
    fclose(fptr);
    printf("Results saved to Mavg_vs_T.txt\n");
    
    free(Mold);
    free(Mnew);
    
    return 0;
}

void randinitialize(int arr[], int L) {
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            arr[i * L + j] = 2 * (rand() % 2) - 1;
        }
    }
}

double metrohastings(int arr1[], int arr2[], double Temp, int L, int N, float J, float dM) {
    double beta = 1.0 / Temp;
    double dE;
    
    
    printf("Equilibrating at T = %.3f... ", Temp);
    fflush(stdout);
    
    int equilibration_steps = N * 2; 
    for (int eq = 0; eq < equilibration_steps; eq++) {
        int i = rand() % L;
        int j = rand() % L;
        
        
        double H = J * (arr2[((i - 1 + L) % L) * L + j] +
                       arr2[((i + 1) % L) * L + j] +
                       arr2[i * L + ((j - 1 + L) % L)] +
                       arr2[i * L + ((j + 1) % L)]);
        
        
        dE = 2.0 * arr2[i * L + j] * H;
        
        
        if (dE <= 0 || ((double)rand() / RAND_MAX) < exp(-beta * dE)) {
            arr2[i * L + j] *= -1; 
        }
    }
    
    
    printf("Measuring... ");
    fflush(stdout);
    
    double M_sum = 0.0;
    int sample_count = 0;
    int sample_interval = L * L; 
    int measurement_steps = N * 3 ; 
    
    for (int n = 0; n < measurement_steps; n++) {
        int i = rand() % L;
        int j = rand() % L;
        
        
        double H = J * (arr2[((i - 1 + L) % L) * L + j] +
                       arr2[((i + 1) % L) * L + j] +
                       arr2[i * L + ((j - 1 + L) % L)] +
                       arr2[i * L + ((j + 1) % L)]);
        
        dE = 2.0 * arr2[i * L + j] * H;
        
        if (dE <= 0 || ((double)rand() / RAND_MAX) < exp(-beta * dE)) {
            arr2[i * L + j] *= -1;
        }
        
        
        if (n % sample_interval == 0) {
            double current_M = Mavg(arr2, L);
            double abs_M = fabs(current_M); 
            M_sum += abs_M;
            sample_count++;
        }
    }
    
    printf("Done.\n");
    return M_sum / sample_count; 
}

void switch_matrix(int **arr1, int **arr2) {
    int *temp = *arr1;
    *arr1 = *arr2;
    *arr2 = temp;
}

float Mavg(int arr[], int L) {
    float sum = 0.0f;
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            sum += arr[i * L + j];
        }
    }
    return sum / (L * L);
}