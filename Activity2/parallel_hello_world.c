//
//  parallel_hello_world.c
//
//  This example uses the Master-Slave programming paradigm where the master process waits
//	a Hello message from all the slaves.

#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
    int rank, size, namelen;
    char host[50], message[50], cont;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(host,&namelen);
    
    if (rank==0){
        MPI_Status status;
        printf("Hello world. I'm %2d of %2d on %s \n\n", rank, size, host);
        fflush(stdout);
        for (cont=1; cont<size;cont++){
            MPI_Recv((void *)message, 50, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Received from %2d: %s \n", status.MPI_SOURCE, message);
            fflush(stdout);
        }
    }
    else {
        sprintf(message,"Hello world. I'm %2d of %2d on %s \n", rank, size, host);
  		MPI_Send((void *)message, 50,MPI_CHAR, 0,1,MPI_COMM_WORLD);
    }
  	
    MPI_Finalize();
    return 0;
}