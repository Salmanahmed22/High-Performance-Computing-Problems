#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double startTime, endTime;

    if (rank == 0) {
        printf("Hello from master process.\n");
        printf("Number of slave processes is %d\n", size - 1);
        startTime = MPI_Wtime();  // Start timer
    }

    int x, y, cntPrime = 0, totalPrime;

    if (rank == 0) {
        printf("Please enter lower bound\n");
        scanf("%d", &x);
        printf("Please enter upper bound\n");
        scanf("%d", &y);
    }

    MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&y, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        int n = y - x + 1;
        int base = n / (size - 1); 
        int remainder = n % (size - 1);
        int start = x + (rank - 1) * base + (rank - 1 < remainder ? rank - 1 : remainder);
        int end = start + base - 1 + (rank - 1 < remainder ? 1 : 0);

        cntPrime = 0;
        for (int num = start; num <= end; num++) {
            int isPrime = 1;
            if (num < 2) isPrime = 0;
            for (int i = 2; i * i <= num && isPrime; i++) {
                if (num % i == 0) isPrime = 0;
            }
            if (isPrime) cntPrime++;
        }

        printf("p%d: calculate partial count of prime numbers from %d to %d and count = %d.\n",
               rank, start, end, cntPrime);
    }

    MPI_Reduce(&cntPrime, &totalPrime, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        endTime = MPI_Wtime();
        printf("Master process announce the total count of prime numbers %d.\n", totalPrime);
        printf("Total execution time: %f seconds\n", endTime - startTime);
        printf("Thanks for using our program\n");
    }

    MPI_Finalize();
    return 0;
}