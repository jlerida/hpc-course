#include "mpi.h"
#include <stdio.h>
int main(int argc, char *argv[]){
	int rank, nprocs;
	float vector[10];
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	if (rank==0) {
	/* Get the input values and write them in “vector”. */
	/* As the values are the same type. */
		int i=0;
		for(i=0;i<10;i++){
			vector[i]=(float)i;
		}
		
		/*send the vector to the processes*/
		for(i=1;i<nprocs;i++){
			MPI_Send((void*)vector, 10, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
		}

	}else {
		MPI_Status status;
		/*recieve the data to work*/
		MPI_Recv((void*)vector, 10, MPI_FLOAT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	}
	char result[500];
	int i=0;
	sprintf(result,"Process %d vector :",rank);
	for(i=0;i<10;i++){
			sprintf(result,"%s %f",result,vector[i]);
	}
	printf("%s\n",result);
	
	MPI_Finalize();
}
