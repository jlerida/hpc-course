#include "mpi.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int rank, nprocs, root;
    struct {
        double value;
        int rank;
    } in, out;
	
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    root=0;
    
    in.value=rank*10;
    in.rank = rank;
    
    MPI_Reduce(&in,&out,1,MPI_DOUBLE_INT,MPI_MAXLOC,root,MPI_COMM_WORLD);
	if(rank==0){
        printf("Rank %d: max=%lf at rank = %d \n", rank,out.value,out.rank);
    }
    
    MPI_Reduce(&in,&out,1,MPI_DOUBLE_INT,MPI_MINLOC,root,MPI_COMM_WORLD);
    if(rank==0){
        printf("Rank %d: min=%lf at rank = %d \n", rank,out.value,out.rank);
    }
    
	MPI_Finalize();
} 

