/* mpi_blockinring.c
 This code implements a MPI aplication that communicates the processes with its neighbours using a ring topology.
 The communication is implemented using blocking functions.
 This solution for short messages but run into a deadlock situation for bigger messages.
 */
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    int numtasks, namelen, rank, dest = 1, tag = 111, source = 0, size, rc, i;
    double start_time=0, elapsed_time=0, acum;
    double *msg;
    char hostname[256];
    MPI_Status status;
    
    if (argc < 2)
    {	printf("Usage: %s size [where size is the number elements (double) to send ]\n", argv[0]);
        return 0;
    }
    
    size = atoi(argv[1]);
    
    msg=(double*)malloc(sizeof(double)*size);
    if(msg==NULL)
    {
        printf("Unable to allocate memory\n");
        return;
    }
    
    MPI_Init (&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);   // get number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);       // get current process id
    MPI_Get_processor_name(hostname, &namelen); // get CPU name
    
    //Initialize the msg buffer to the rank id.
    for (i = 0; i < size; i++)
        msg[i] = rank;
    
    //Define as Source the left neighbour
    if (rank == 0)
        source=numtasks-1;
    else
        source=rank-1;

    //Define the destiny the rigth neighbour
    if(rank==numtasks-1)
        dest=0;
    else
        dest=rank+1;

    start_time = MPI_Wtime();
    acum=0;
    for (i=0; i<numtasks; i++) {
        if (rank==0) printf("Rank %d (%s) sending data (%g) to rank %d\n",rank, hostname, msg[0], dest);
        MPI_Send(msg, size, MPI_DOUBLE, dest, tag,MPI_COMM_WORLD);
        MPI_Recv (msg, size, MPI_DOUBLE, source, tag, MPI_COMM_WORLD,&status);
        acum = acum + msg[0];
        if (rank==0) printf("Rank %d received data (%g) from rank %d (acum=%g)\n",rank,msg[0],source,acum);
    }

    elapsed_time = MPI_Wtime() - start_time;
    printf(" Rank %d: Elapsed time to send %6d double(s) across a ring made up by %2d (acum=%g) in %g ms\n", rank, size, numtasks, acum, elapsed_time*1e03);
    
    MPI_Finalize ();
}
