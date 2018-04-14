//
//  mpi_time.c
//
//  This example uses the Master-Slave programming paradigm where the master process waits
//	a message from each slaves. This example print on the screen the elapsed time.

#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){
    int rank, size, namelen;
    char host[50], message[50], cont;
    double start_time=0, finish_time=0, elapsed_time=0;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(host,&namelen);
    
    if (rank==0){
    	start_time = MPI_Wtime();
        MPI_Status status;
        printf("\nHello world. I'm %d of %d on %s \n\n", rank, size, host);
        fflush(stdout);
        for (cont=1; cont<size;cont++){
            MPI_Recv((void *)message, 50, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Received from %d: %s \n", status.MPI_SOURCE, message);
            fflush(stdout);
        }
		finish_time = MPI_Wtime();
        elapsed_time = finish_time - start_time;    
		printf("Elapsed time --- %g ms\n", elapsed_time*1e03);  
    }
    else {
        sprintf(message,"Hello world. I'm %d of %d on %s \n", rank, size, host);
  		MPI_Send((void *)message,50,MPI_CHAR,0,1,MPI_COMM_WORLD);
    }
  	
    MPI_Finalize();
    return 0;
}