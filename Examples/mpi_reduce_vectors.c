#include "mpi.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int rank, nprocs;
	int vector[20];
	int sum[20];
	int i;
	int result;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	srand(time(NULL)*rank);
	//each processor initializes the vector
	for(i=0;i<20;i++){
		vector[i]=rand()%10;
	}
    
//    printf("\nProcess %d vector: ",rank);
//    for(i=0;i<20;i++){
//        printf(" %d-%2d ",rank,vector[i]);
//    }
//    printf("\n\n");

	//Apply MPI_SUM for each process and store result to sum
	MPI_Reduce((void*)vector,&sum,20,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	
	if(rank==0){
		//process 0 obtains the results, calculates the final sum and prints
		result=0;
		for(i=0;i<20;i++){
			result+=sum[i];
//            printf(" %d ",sum[i]);
		}
		printf("sum = %d\n",result);
	}
	MPI_Finalize();
} 

