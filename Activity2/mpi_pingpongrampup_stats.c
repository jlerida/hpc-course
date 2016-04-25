#include "mpi.h"
#include <stdio.h>

#define SIZE  524288
#define KBYTE 1024
#define MBYTE 1024*1024
#define GBYTE 1024*1024*1024

main(int argc, char *argv[])  {
    int numtasks, namelen, rank, dest, source, rc, count, tag=1, i, size, loops;
    double start_time=0, elapsed_time=0, st1=0, fn1=0, el1=0, st2=0, el2=0, elacum=0; /* for time measurement */
    double inmsg[SIZE], outmsg[SIZE], msgsizebytes=0, transferMSGtime=0;
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
        printf("process %2d of %2d (node %s)\n", rank, numtasks, hostname );

        st1 = MPI_Wtime();
        i=0;
        elacum=0;
        rc = MPI_Send(&outmsg, size, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
        rc = MPI_Recv(&inmsg, size, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &Stat);
        rc = MPI_Get_count(&Stat, MPI_DOUBLE, &count);
        fn1 = MPI_Wtime();
        el1 = fn1 - st1;
        printf("Round %6d --- Received %2d double(s) in %g ms\n",i+1, count, el1*1e03);
        printf("%g ", el1*1e03);
        for (i=1; i<loops; i++) {
            st2 = MPI_Wtime();
            rc = MPI_Send(&outmsg, size, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
            rc = MPI_Recv(&inmsg, size, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &Stat);
            el2 = MPI_Wtime() - st2;
            elacum = elacum + el2;
            rc = MPI_Get_count(&Stat, MPI_DOUBLE, &count);
//            printf("Round %6d --- Received %2d double(s) in %g ms\n",i+1, count, el2*1e03);
            printf("%g ", el2*1e03);
        }
        
        elapsed_time = (loops>1) ? MPI_Wtime() - st1 : el1;
        
        printf(" Statistics\n");
        printf(" %6d  ping-pongs     with %6d double(s) %g ms\n", loops, size, elapsed_time*1e03);
        printf(" 1st loop               with %6d double(s) %g ms\n", size, el1*1e03);
        if (loops>1) {
            printf(" Average xloop (%6d) with %6d double(s) %g ms\n", loops-1, size, (elacum/(loops-1))*1e03);
            transferMSGtime = elacum/(2*(loops-1));
        }
        else {
            elacum = elapsed_time;
            transferMSGtime = elacum/2;
        }
        
        msgsizebytes    = size*sizeof(MPI_DOUBLE);
        printf(" Average xloop (%6d) with %6d double(s) %9.4f ms\n", loops  , size, (elapsed_time/loops)*1e03);
        printf(" Average message time of                      %g ms\n", elacum/(2*loops)*1e03);
        printf(" Average    Byte time of                      %g ms\n", elacum/(2*loops*size*sizeof(MPI_DOUBLE))*1e03);
        printf(" MSGsizebytes    = %9.4f\n",msgsizebytes);
        printf(" transferMSGtime = %9.4f\n",transferMSGtime);
        printf(" Bandwidth (%g Bytes/%g sec) %g MBytes/sec\n", msgsizebytes,transferMSGtime,(msgsizebytes/(MBYTE))/transferMSGtime);
    }
    
    else if (rank == 1) {
        dest = 0;
        source = 0;
        printf( "process %d of %d (node %s)\n", rank, numtasks, hostname );
        for (i=0; i<loops; i++) {
            rc = MPI_Recv(&inmsg, size, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &Stat);
            rc = MPI_Send(&outmsg, size, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
            rc = MPI_Get_count(&Stat, MPI_DOUBLE, &count);
            fflush(stdout);            
        }
    }
    
    MPI_Finalize();
}
