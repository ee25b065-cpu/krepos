#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int random_point_generator_and_checker(int N, float L);

int main(int argc, char *argv[]) {
        if (argc != 3) {
        fprintf(stderr, "Error: Insufficient arguments\nUsage: %s <N> <L>\n", argv[0]);// Check for correct number of arguments
        return 1;
    }
    
    int N = atoi(argv[1]);
    float L = atof(argv[2]);
    
        if (N <= 0 || L <= 0) {
        fprintf(stderr, "Error: Both N and L must be positive\n");// Check that both inputs are positive
        return 1;
    }
    
    srand(time(NULL));

    float pi_estimate = 4.0 *(random_point_generator_and_checker(N,L)) / N;//Esmate pi using the ratio of points in circle to total points

    float error = fabs(pi_estimate - M_PI);

    printf("%d %f %f\n", N, pi_estimate, error);
    
    return 0;
 
}    

//random point generator formatted by claude.ai after given pseodo code.
int random_point_generator_and_checker(int N, float L) {
    int points_in_circle = 0;
    for (int i = 0; i < N; i++) {
        float x = ((float)rand() / RAND_MAX) * L;  
        float y = ((float)rand() / RAND_MAX) * L;
        
        if (x*x + y*y <= L*L) {
            points_in_circle++; // Check if point is within quarter circle of radius L
        }                       // Distance from origin: sqrt(x^2 + y^2)
                                // Point is inside if distance <= L, or equivalently x^2 + y^2 <= L^2      
    }
    return points_in_circle;
    }