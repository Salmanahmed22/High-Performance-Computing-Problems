#include <stdio.h>
#include <omp.h>
#include <math.h>

int main() {
    int i;
    int n;
    printf("Enter the size of the array: \n");
    scanf("%d", &n);

    printf("Enter the array: \n");
    
    int array[n];
    
    for(int i=0; i<n; i++){
      scanf("%d", &array[i]);
    }
    double mean = 0.0, variance = 0.0, stddev = 0.0;
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < n; i++) {
        sum += array[i];
    }

    mean = sum / n;

    double sum_sq_diff = 0.0;
    #pragma omp parallel for reduction(+:sum_sq_diff)
    for (i = 0; i < n; i++) {
        sum_sq_diff += (array[i] - mean) * (array[i] - mean);
    }

    variance = sum_sq_diff / n;
    stddev = sqrt(variance);

    printf("Standard Deviation: %.6f\n", stddev);

    return 0;
}