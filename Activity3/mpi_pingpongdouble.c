//
//  mpi_pingpongdouble.c
//
//  Example PingPong implementation for double values. 
//	This example consist in two processes that send and receive a set of double values
//	during a specific number of iterations. The number of values and iterations is settled
//	by parameters.

#include <mpi.h>
#include <stdio.h>

#define SIZE 262144

main(int argc, char *argv[])  {
    int numtasks, namelen, rank, dest, source, rc, count, tag=1, i, size, loops;
    double start_time, elapsed_time; /* for time measurement */
    double inmsg[SIZE], outmsg[SIZE];
    char hostname[256];
    MPI_Status Stat;
    
    if (argc < 3)
    {	printf("Usage: %s size n [where size is the number of elements and n is number of ping-pong steps ]\n", argv[0]);
        return 0;
    }
    
    for (i=0; i<SIZE; i++) {
        outmsg[i]=-1;
    }

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);   // get number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);       // get current process id
    MPI_Get_processor_name(hostname, &namelen); // get CPU name
    
    size = atoi(argv[1]);
    loops= atoi(argv[2]);
    
    i=0;
    if (rank == 0) {
        dest = 1;
        source = 1;
        for (i=0; i<size; i++) {
            outmsg[i]=i+0.5;
        }
        printf("PingPong program with %d double elements and %d loops\n",size, loops);
        printf("process %d of %d (node %s)\n", rank, numtasks, hostname );
        start_time = MPI_Wtime();
        for (i=0; i<loops; i++) {
            rc = MPI_Send(&outmsg, size, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
            rc = MPI_Recv(&inmsg, size, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &Stat);
            rc = MPI_Get_count(&Stat, MPI_DOUBLE, &count);
            printf("Round %2d --- Task %d: Received %d double(s) from task %d with tag %d \n",
                   i+1, rank, count, Stat.MPI_SOURCE, Stat.MPI_TAG);
            fflush(stdout);
        }
        elapsed_time = MPI_Wtime() - start_time;
        
//        printf(" The size of doubles is %d", sizeof(MPI_DOUBLE));
        printf(" %d ping-pongs with %d double(s) in %g ms\n", loops, size, elapsed_time*1e03);
        printf(" Average message time of %g ms\n", elapsed_time/(2*loops)*1e03);
        printf(" Average    Byte time of %g ms\n", elapsed_time/(2*loops*size*sizeof(MPI_DOUBLE))*1e03);
    }
    
    else if (rank == 1) {
        dest = 0;
        source = 0;
        printf( "process %d of %d (node %s)\n", rank, numtasks, hostname );
        for (i=0; i<loops; i++) {
            rc = MPI_Recv(&inmsg, size, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &Stat);
            rc = MPI_Send(&inmsg, size, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
            rc = MPI_Get_count(&Stat, MPI_DOUBLE, &count);
            printf("Round %2d --- Task %d: Received %d double(s) from task %d with tag %d \n",
                   i+1, rank, count, Stat.MPI_SOURCE, Stat.MPI_TAG);
            fflush(stdout);            
        }
    }
    
    MPI_Finalize();
}
