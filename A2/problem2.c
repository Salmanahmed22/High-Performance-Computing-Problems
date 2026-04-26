#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 5
#define M 3

void initializeMatrix(int *mat) {
    for (int i = 0; i < N * M; ++i) {
        mat[i] = rand() % 9 + 1;
    }
}

void printMatrix(int *mat) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            printf("%3d ", mat[i * M + j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    int rank, size;
    int *A = NULL, *B = NULL, *C = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rowsFrProc = (N + size - 1) / size;
    int chunkSize = rowsFrProc * M;
    int paddRows = rowsFrProc * size;

    int *subA = malloc(sizeof(int) * chunkSize);
    int *subB = malloc(sizeof(int) * chunkSize);
    int *subC = malloc(sizeof(int) * chunkSize);

    if (rank == 0) {
        A = malloc(sizeof(int) * paddRows * M);
        B = malloc(sizeof(int) * paddRows * M);
        C = malloc(sizeof(int) * paddRows * M);

        
        initializeMatrix(A);
        initializeMatrix(B);

        for (int i = N * M; i < paddRows * M; ++i) {
            A[i] = 0;
            B[i] = 0;
        }

        printf("Matrix A:\n");
        printMatrix(A);
        printf("Matrix B:\n");
        printMatrix(B);

    }


    MPI_Scatter(A, chunkSize, MPI_INT, subA, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, chunkSize, MPI_INT, subB, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);


    for (int i = 0; i < chunkSize; ++i) {
        subC[i] = subA[i] + subB[i];
    }

    MPI_Gather(subC, chunkSize, MPI_INT, C, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Result Matrix C (A + B):\n");
        printMatrix(C);

        free(A);
        free(B);
        free(C);
    }

    free(subA);
    free(subB);
    free(subC);

    MPI_Finalize();
    return 0;
}