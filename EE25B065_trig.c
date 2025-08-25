/*EE25B065,EE25B066,EE25B075*/

// Write a C-program that will
// take as command line input an integer N and a decimal value for theta
// check that the input is a positive integer, else throw and error and exit
// convert theta into radians
// calculate the trigonometric functions sin(theta) and cos(theta) using the inbuilt functions in math.h
// use functions mysine(N,theta_rad) and mycosine(theta_rad) to calculate the values of sin(theta) and cos(theta) from its Taylor series, up to N terms. e.g. sin(x) = x - x^3/6 + ....
// print out the value of N and the error_sine and error_cosine. The error is the difference between the values returned by the inbuilt math functions and your functions
// Write a shell script to call the C-executable in a loop for increasing values of N, and theta = 45 degrees. The output of your script will then be 3 columns, N error_sine error_cosine

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double convert(int theta) {
    return (theta * M_PI / 180.0);
}

int isInteger(const char *str) {
    // Handle empty string
    if (*str == '\0') return 0;

    // Allow optional + or - at the beginning
    if (*str == '+' || *str == '-') str++;

    // At least one digit should be present
    if (!*str) return 0;

    while (*str) {
        if (!isdigit((unsigned char)*str)) {
            return 0; // Not an integer
        }
        str++;
    }
    return 1; // Valid integer
}

// Function to calculate sine using Taylor series with an iterative approach
double mysine(int N, double theta_rad) {
    double sin_val = 0.0;
    double term = theta_rad; // The first term (n=0) is x
    sin_val += term;

    for (int n = 1; n < N; n++) {
        // Calculate the next term from the previous one
        term = -term * theta_rad * theta_rad / ((2.0 * n) * (2.0 * n + 1.0));
        sin_val += term;
    }
    return sin_val;
}

// Function to calculate cosine using Taylor series with an iterative approach
double mycosine(int N, double theta_rad) {
    double cos_val = 0.0;
    double term = 1.0; // The first term (n=0) is 1
    cos_val += term;

    for (int n = 1; n < N; n++) {
        // Calculate the next term from the previous one
        term = -term * theta_rad * theta_rad / ((2.0 * n - 1.0) * (2.0 * n));
        cos_val += term;
    }
    return cos_val;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <number1> <number2>\n", argv[0]);
        return 1;
    }
    int N;
    double theta;
    N = atoi(argv[1]);
    theta = atof(argv[2]);

    if ((!(isInteger(argv[1]))) || (N <= 0)) {
        fprintf(stderr, "Error: N must be a positive integer.\n");
        return 1;
    }

    double theta_rad = convert(theta);
    double sine = sin(theta_rad);
    double cosine = cos(theta_rad);


    double tay_sine = mysine(N, theta_rad);
    double tay_cosine = mycosine(N, theta_rad);
    

    double error_sine = fabs(sine - tay_sine);
    double error_cosine = fabs(cosine - tay_cosine);
    printf("%d %.10e %.10e\n", N, error_sine, error_cosine);


    return 0;
}