#include "mpi.h"
#include <stdio.h>

main(int argc, char *argv[])  {
    int numtasks, namelen, rank, dest, source, rc, count, tag=1, i;
    char inmsg, outmsg='x';
    char hostname[256];
    MPI_Status Stat;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);   // get number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);       // get current process id
    MPI_Get_processor_name(hostname, &namelen); // get CPU name
    
    if (rank == 0) {
        dest = 1;
        source = 1;
        printf( "process %d of %d (node %s)\n", rank, numtasks, hostname );
        for (i=0; i<50; i++)
        {
            rc = MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
            rc = MPI_Recv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
            rc = MPI_Get_count(&Stat, MPI_CHAR, &count);
            printf("Round %d --- Task %d: Received %d char(s) from task %d with tag %d \n",
                   i+1, rank, count, Stat.MPI_SOURCE, Stat.MPI_TAG);
            fflush(stdout);
        }
    }
    
    else if (rank == 1) {
        dest = 0;
        source = 0;
        printf( "process %d of %d (node %s)\n", rank, numtasks, hostname );
        for (i=0; i<50; i++) {
            rc = MPI_Recv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
            rc = MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
            rc = MPI_Get_count(&Stat, MPI_CHAR, &count);
            printf("Round %d --- Task %d: Received %d char(s) from task %d with tag %d \n",
                   i+1, rank, count, Stat.MPI_SOURCE, Stat.MPI_TAG);
            fflush(stdout);            
        }
    }
    
    MPI_Finalize();
}
