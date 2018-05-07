#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv ){
	int rank,nprocs;
	struct {float data1; int data2;char* data3;} data;
	MPI_Datatype mydataStruct;
	int blocks[3];
	MPI_Aint indices[3];
	MPI_Datatype MPItypes[3];

	MPI_Init( &argc, &argv );

	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &nprocs);

	/* One value of each type */
	blocks[0] = 1;
	blocks[1] = 1;
	blocks[2] =10;
	/* The base types */
	MPItypes[0] = MPI_FLOAT;
	MPItypes[1] = MPI_INT;
	MPItypes[2] = MPI_CHAR;
	/* The locations of each element */
	MPI_Get_address( &data.data1, &indices[0] );
	MPI_Get_address( &data.data2, &indices[1] );
	MPI_Get_address( &data.data3, &indices[2] );
	/* Make relative */
	MPI_Aint baseaddres=indices[0];
	indices[0] = indices[0] - baseaddres;
	indices[1] = indices[1] - baseaddres;
	indices[3] = indices[3] - baseaddres;
	MPI_Type_create_struct( 3, blocks, indices, MPItypes, &mydataStruct );
	MPI_Type_commit( &mydataStruct );

   
    if (rank == 0){
        data.data1=25.30;
		data.data2=7;
		data.data3="String 10!";
		int i;
		for(i=1;i<nprocs;i++){
	  		MPI_Send(&data, 1, mydataStruct,i, 0, MPI_COMM_WORLD );
		}
	} else {
		MPI_Status status;
		MPI_Recv(&data,1,mydataStruct,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
	}
        
	printf( "Process %d got %f, %d and %s\n", rank, data.data1, data.data2,data.data3 );
   
    /* Clean up the type */
    MPI_Type_free( &mydataStruct );
    MPI_Finalize( );
    return 0;
}
