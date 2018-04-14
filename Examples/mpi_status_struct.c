#include "mpi.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int rank, nprocs, count, i;
    float in[200],out[200];
    MPI_Status status;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    if (rank==0) {
        for (i=0; i<100; i++) out[i]=i;
        MPI_Send(out,100,MPI_FLOAT,1,55,MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(in,200,MPI_FLOAT,MPI_ANY_SOURCE,55,MPI_COMM_WORLD,&status);
        MPI_Get_count(&status,MPI_FLOAT,&count);
        printf("Rank %d : Got %d data elements from processor %d.\n", rank, count, status.MPI_SOURCE);
    }
	MPI_Finalize();
} 

