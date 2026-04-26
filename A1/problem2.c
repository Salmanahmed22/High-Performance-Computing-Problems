#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char caesarCipher(char c, int operation) {
    if (operation == 1) { //Encode
        if (c >= 'A' && c <= 'Z') {
            int idx = c - 'A';
            int x = (idx + 3) % 26;
            return 'A' + x;
        } else if (c >= 'a' && c <= 'z') {
            int idx = c - 'a';
            int x = (idx + 3) % 26;
            return 'a' + x;
        }
    } else { //Decode
        if (c >= 'A' && c <= 'Z') {
            int idx = c - 'A';
            int x = (idx - 3) % 26;
            if (x < 0) {
                x += 26;
            }
            return 'A' + x;
        } else if (c >= 'a' && c <= 'z') {
            int idx = c - 'a';
            int x = (idx - 3) % 26;
            if (x < 0) {
                x += 26;
            }
            return 'a' + x;
        }
    }
    return c;
}

int main()
{
    MPI_Init(NULL, NULL);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int number_of_processes;
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);

    int choice;
    int operation;
    int size;

    if (rank == 0)
    {
        printf("Choose whether console (1) or Read file input(2): ");
        fflush(stdout);
        scanf("%d", &choice);

        printf("Choose whether Encode (1) or Decode(2): ");
        fflush(stdout);
        scanf("%d", &operation);
        
        char *input;
        if(choice == 1){
            printf("Enter the size of input: ");
            fflush(stdout);
            scanf("%d", &size);
            input = (char*)malloc(size + 1);

            printf("Enter the input string: ");
            fflush(stdout);
            scanf("%s", input);
        }
        else{
            FILE *p = fopen("/home/Omar/Desktop/Problem2/name.txt","r");
            if (p == NULL) {
                perror("Can't open file");
                return 1;
            }
            char temp[100];
            fscanf(p, "%s", temp);
            fclose(p);
            size = strlen(temp);
            input = (char*)malloc(size + 1);
            strcpy(input, temp);
        }

        int portions = size / (number_of_processes - 1);
        int remainder = size % (number_of_processes -1);

        int i;
        for (i = 1; i < number_of_processes; i++)
        {
            int start_index = (i-1) * portions;
            int length;

            if (i == number_of_processes - 1)
            {
                length = portions + remainder;
            }
            else
            {
                length = portions;
            }

            MPI_Send(&size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&operation, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(input + start_index, length, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
        //gathering
        for (i = 1; i < number_of_processes; i++)
        {
            int start_index = (i-1) * portions;
            int length;

            if (i == number_of_processes - 1)
            {
                length = portions + remainder;
            }
            else
            {
                length = portions;
            }
            MPI_Status status;
            MPI_Recv(input + start_index , length , MPI_CHAR , i , 0 ,MPI_COMM_WORLD,&status);

        }
        printf("output: %s\n",input);
    }
    else
    {
        MPI_Status status;
        MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&operation, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        int portions = size / (number_of_processes - 1);
        int remainder = size % (number_of_processes - 1);
        int length;

        if (rank == number_of_processes - 1)
        {
            length = portions + remainder;
        }
        else
        {
            length = portions;
        }

        //receive scattered arr from diff proccesses
        char* temp = (char*)malloc(length + 1);
        MPI_Recv(temp, length, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        temp[length] = '\0';


        //encoding or decoding loop
        int i;
        for(i = 0 ; i < length; i++)
        {
            char newChar = caesarCipher(temp[i],operation);
            temp[i] = newChar;
        }
        MPI_Send(temp, length, MPI_CHAR, 0, 0, MPI_COMM_WORLD);


        free(temp);
    }

    MPI_Finalize();
    return 0;
}