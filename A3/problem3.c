#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Hello from master process.\n");
        printf("Number of slave processes is %d\n", size - 1);
        printf("Please enter size of the array...\n");
    }

    int n;
    if (rank == 0) {
        scanf("%d", &n);
        printf("Please enter array elements ...\n");
        for (int i = 1; i < size; i++) {
            MPI_Send(&n, 1, MPI_INT, i, 99, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&n, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    int* arr = NULL;
    if (rank == 0) {
        arr = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }
    }
  
    int localSize = n / (size - 1); // 12 / 5 = 2
    int remainder = n % (size - 1); // 12 % 5 = 2
    int startIndex = (rank - 1) * localSize + (rank - 1 < remainder ? rank - 1 : remainder);
    int endIndex = startIndex + localSize + (rank - 1 < remainder ? 1 : 0);
    int actualLocalSize = endIndex - startIndex;

    int* localArr = malloc(actualLocalSize * sizeof(int));

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            int start = (i - 1) * localSize + (i - 1 < remainder ? i - 1 : remainder);
            int end = start + localSize + (i - 1 < remainder ? 1 : 0);
            MPI_Send(&arr[start], end - start, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(localArr, actualLocalSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int localSum = 0;
        #pragma omp parallel for reduction(+:localSum)
        for (int i = 0; i < actualLocalSize; i++) {
            localSum += localArr[i];
        }
        printf("Hello from slave#%d sum in my partition is %d.\n", rank, localSum);
        MPI_Send(&localSum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    int globalSum = 0;
    if (rank == 0) {
        int recvLocalSum;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&recvLocalSum, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            globalSum += recvLocalSum;
        }
        printf("Master process announce the final sum is %d.\n", globalSum);
        printf("Thanks for using our program\n");
    } 

    free(localArr);
    if (rank == 0) {
        free(arr);
    }
    MPI_Finalize();
    return 0;
}