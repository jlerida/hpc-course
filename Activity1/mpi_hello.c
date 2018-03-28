//
//  mpi_hello.c
//
//  Simple MPI example
//

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
    int rank, size;
    char hostname[256];
    int namelen;
    
    MPI_Init (&argc, &argv);      /* starts MPI */
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);        // get current process id
    MPI_Comm_size (MPI_COMM_WORLD, &size);        // get number of processes
    MPI_Get_processor_name(hostname, &namelen);   // get CPU name

    printf( "Hello world from process %d of %d (node %s)\n", rank, size, hostname );
    MPI_Finalize();
    return 0;
}
