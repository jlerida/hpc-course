#include "mpi.h"
#include <stdio.h>
int main(int argc, char *argv[]){
	int rank, nprocs;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	char buffer[100]; // buffer to store the data(MPI_Pack/MPI_Unpack) 
	int position; // the position of the buffer
	
	float data1;
	int data2;

	if (rank== 0) {
		//define the Data
		data1=25.30;
		data2=10;

		//pack the data into buffer 
		position=0; // Start at beginning of buffer 
		MPI_Pack(&data1, 1, MPI_FLOAT, buffer, 100, &position, MPI_COMM_WORLD);
		// Position has been incremented by sizeof(float) bytes 
		MPI_Pack(&data2, 1, MPI_INT, buffer, 100, &position, MPI_COMM_WORLD);
		// Send the contents of buffer to processors 
		int i=0;
		for(i=1;i<nprocs;i++){
			MPI_Send((void*)buffer, 100, MPI_PACKED, i,0, MPI_COMM_WORLD);
		}
	}else {
		//Recieve the message
		MPI_Status status;
		MPI_Recv((void*)buffer,100,MPI_PACKED,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		//Unpack the contents of buffer 
		position=0;
		MPI_Unpack(buffer, 100, &position, &data1, 1, MPI_FLOAT, MPI_COMM_WORLD);
		MPI_Unpack(buffer, 100, &position, &data2, 1, MPI_INT, MPI_COMM_WORLD);

	}
	char result[500];
	int i=0;
	sprintf(result,"Process %d data %f %d",rank,data1,data2);
	printf("%s\n",result);
	
	MPI_Finalize();
}
