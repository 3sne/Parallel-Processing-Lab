#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    int rank, size, len;
    char senderWord[256];
    char recvWord[256];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if ( rank == 0 ) {

        printf("[Rank %d] Enter a word >> ", rank);
        scanf("%s", senderWord);
        len = strlen(senderWord);
        MPI_Ssend(&len, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Ssend(senderWord, len, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
        MPI_Recv(senderWord, len, MPI_CHAR, 1, 2, MPI_COMM_WORLD, &status);
        printf("[Rank %d] Got toggled word from slave >> %s\n", rank, senderWord);

    } else if ( rank == 1 ) {

        MPI_Recv(&len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(recvWord, len, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
        printf("[Rank %d] Got this word >> %s\n", rank, recvWord);
        int i = 0, ascii;
        while(recvWord[i] != '\0') {
            ascii = (int)recvWord[i];
            if ( ascii >= 65 && ascii <= 90 ) { //Upper -> Lower
                ascii += 32;
            } else if ( ascii >= 97 && ascii <=122 ) { //Lower -> Upper
                ascii -= 32;
            }
            recvWord[i] = (char)ascii;
            i++;
        }
        printf("[Rank %d] Converted it to >> %s\n", rank, recvWord);
        printf("[Rank %d] Sending it back to Master...\n", rank);
        MPI_Ssend(recvWord, len, MPI_CHAR, 0, 2, MPI_COMM_WORLD);

    }

    MPI_Finalize();
    return 0;
}
