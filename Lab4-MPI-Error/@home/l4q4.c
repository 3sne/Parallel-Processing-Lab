#include<mpi.h>
#include<stdio.h>
#include<string.h>

int checkErrRoutine(int err){
    char errStr[1024];
    int errClass, len;
    MPI_Error_class(err, &errClass);
    len = strlen(errStr);
    MPI_Error_string(errClass, errStr, &len);
    fprintf(stderr, "%s :%d\n", errStr, len);
}
 
int main(int argc, char *argv[]) {
    int rank, size, err;
    err = MPI_Init(&argc, &argv);
    MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
    checkErrRoutine(err);
    err = MPI_Comm_rank(69420, &rank);
    checkErrRoutine(err);
    err = MPI_Comm_size(MPI_COMM_WORLD, &size);
    checkErrRoutine(err);
    int a[256];
    err = MPI_Bcast(a, 2, MPI_INT, 102, MPI_COMM_WORLD);
    checkErrRoutine(err);
    MPI_Finalize();
    return 0;
}