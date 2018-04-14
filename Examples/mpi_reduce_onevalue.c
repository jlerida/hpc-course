#include "mpi.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int rank, nprocs;
    int value;
    int sumtotal;
    int prodtotal;
	
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    value = (rank+1)*10;
    
    MPI_Reduce(&value,&sumtotal,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Reduce(&value,&prodtotal,1,MPI_INT,MPI_PROD,0,MPI_COMM_WORLD);
	
	if(rank==0){
        printf("Collective sum result %d\n", sumtotal);
        printf("Collective product results %d\n", prodtotal);
    }
	MPI_Finalize();
} 

