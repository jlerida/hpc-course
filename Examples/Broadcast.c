#include "mpi.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int rank, nprocs;
	int param;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	srand(time(NULL)*rank);
    
    if(rank==5) param = 23;

    MPI_Bcast(&param,1,MPI_INT,5,MPI_COMM_WORLD);
    printf("\nProcess %d received by broadcast value %d",rank,param);
    
	MPI_Finalize();
} 

