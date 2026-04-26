#include <stdio.h>
#include <omp.h>
#include <time.h>

int main() {
    int n;

    printf("Enter the size of the matrix and vector (n): ");
    scanf("%d", &n);

    int matrix[n][n];
    int vector[n];
    int result[n];

    printf("Matrix A:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &matrix[i][j]);

    printf("Vector V:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &vector[i]);
        
    clock_t start_time = clock();
    
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    
    clock_t end_time = clock();
    printf("time needed: %f\n", (double)(end_time - start_time)/2500000); //clock_per_sec
    
    printf("Resulting vector:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", result[i]);
    printf("\n");

    return 0;
}