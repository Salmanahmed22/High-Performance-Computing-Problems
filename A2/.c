#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int x, y, cntPrime = 0, totalPrime = 0;
    double startTime, endTime;

    if (rank == 0) {
        printf("Hello from master process.\n");
        printf("Number of slave processes is %d\n", size - 1);

        printf("Please enter lower bound\n");
        scanf("%d", &x);
        printf("Please enter upper bound\n");
        scanf("%d", &y);

        startTime = MPI_Wtime();

        
        for (int i = 1; i < size; i++) {
            MPI_Send(&x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&y, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        for (int i = 1; i < size; i++) {
            int partial;
            MPI_Recv(&partial, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            totalPrime += partial;
        }

        endTime = MPI_Wtime();

        printf("Master process announces the total count of prime numbers: %d\n", totalPrime);
        printf("Total execution time: %f seconds\n", endTime - startTime);
        printf("Thanks for using our program\n");
    } else {
        MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&y, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        int n = y - x + 1;
        int base = n / (size - 1); 
        int remainder = n % (size - 1);
        int start = x + (rank - 1) * base + (rank - 1 < remainder ? rank - 1 : remainder);
        int end = start + base - 1 + (rank - 1 < remainder ? 1 : 0);

        cntPrime = 0;
        for (int num = start; num <= end; num++) {
            int isPrime = 1;
            if (num <= 1) isPrime = 0;
            for (int i = 2; i * i <= num && isPrime; i++) {
                if (num % i == 0) isPrime = 0;
            }
            if (isPrime) cntPrime++;
        }

        printf("p%d: calculate partial count of prime numbers from %d to %d and count = %d.\n",
               rank, start, end, cntPrime);

        MPI_Send(&cntPrime, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}