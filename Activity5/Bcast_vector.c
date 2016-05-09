#include "mpi.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int rank, nprocs;
	int vector[20];
	int i;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	srand(time(NULL)*rank);
    
    if(rank==3){
        //process 3 generates a vector of random numbers
        for(i=0;i<20;i++){
            vector[i]=rand()%10;
        }
        //Send the buffer to all processes in the MPI_COMM_WORLD with a Broadcast
        MPI_Bcast((void*)vector,20,MPI_INT,3,MPI_COMM_WORLD);
    }

    MPI_Bcast((void*)vector,20,MPI_INT,3,MPI_COMM_WORLD);
    printf("\nProcess %d vector: ",rank);
    for(i=0;i<20;i++){
        printf(" %2d ",vector[i]);
    }
    printf("\n");
    fflush(stdout);
    
	MPI_Finalize();
} 

