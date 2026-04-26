#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Hello from master process.\n");
        printf("Number of slave processes is %d\n", size - 1);
        printf("Please enter size of array...\n");
    }

    int n;
    if (rank == 0) {
        scanf("%d", &n);
        printf("Please enter array elements ...\n");
        int i;
        for (i = 1; i < size; i++) {
            MPI_Send(&n, 1, MPI_INT, i, 99, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&n, 1, MPI_INT, 0, 99, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }


    int* arr = NULL;
    if (rank == 0) {
        arr = malloc(n * sizeof(int));
        int i;
        for (i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }
    }

    // Calculate local sizes and distribute the un-regular size from the first processes  
    int localSize = n / (size - 1); // 12 / 5 = 2
    int remainder = n % (size - 1); // 12 % 5 = 2
    int startIndex = (rank - 1) * localSize + (rank - 1 < remainder ? rank - 1 : remainder);
    int endIndex = startIndex + localSize + (rank - 1 < remainder ? 1 : 0);
    int actualLocalSize = endIndex - startIndex;

    int* localArr = malloc(actualLocalSize * sizeof(int));

    if (rank == 0) {
        // Distribute data to other processes
        int i;
        for (i = 1; i < size; i++) {
            int start = (i - 1) * localSize + (i - 1 < remainder ? i - 1 : remainder);
            int end = start + localSize + (i - 1 < remainder ? 1 : 0);
            MPI_Send(&arr[start], end - start, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        // Receive data
        MPI_Recv(localArr, actualLocalSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // Find local maximum and its index
        int localMax = localArr[0];
        int localMaxIndex = 0;
        int i;
        for (i = 1; i < actualLocalSize; i++) {
            if (localArr[i] > localMax) {
                localMax = localArr[i];
                localMaxIndex = i;
            }
        }
        printf("Hello from slave#%d Max number in my partition is %d and index is %d.\n", rank, localMax, localMaxIndex);
        MPI_Send(&localMax, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&localMaxIndex, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }

    // Gather local maxima and their indices at the master process
    int globalMax, globalMaxIndex;
    if (rank == 0) {
        int recvLocalMax, recvLocalMaxIndex;
        globalMax = -2147483648; // Initialize to the smallest integer
        int i;
        for (i = 1; i < size; i++) {
            MPI_Recv(&recvLocalMax, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&recvLocalMaxIndex, 1, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int globalIndex = (i - 1) * localSize + (i - 1 < remainder ? i - 1 : remainder) + recvLocalMaxIndex;
            if (recvLocalMax > globalMax) {
                globalMax = recvLocalMax;
                globalMaxIndex = globalIndex;
            }
        }
        printf("Master process announce the final max which is %d and its index is %d.\n", globalMax, globalMaxIndex);
        printf("Thanks for using our program\n");
    } 

    free(localArr);
    if (rank == 0) {
        free(arr);
    }
    MPI_Finalize();
    return 0;
}