#include "Distributed.h"

Distributed::Distributed():Analysis(){
	program = "Dist_MonteAnneal";
}

void Distributed::start(string filename){
	Analysis::start(filename);
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	char hostbuff[100];
	gethostname(hostbuff,99);
	hostname = string(hostbuff);
	program = program + "_" + to_string(size);
}

void Distributed::sendMatrix(MatrixXd& matrix,int dest){
	int tag = 0;
	MPI_Status status;
	MPI_Send(matrix.data(),matrix.rows()*matrix.cols(),MPI_DOUBLE,dest,tag,MPI_COMM_WORLD);
}

void Distributed::recvMatrix(MatrixXd& matrix,int src){
	int tag = 0;
	MPI_Status status;
	MPI_Recv(matrix.data(),matrix.rows()*matrix.cols(),MPI_DOUBLE,src,tag,MPI_COMM_WORLD,&status);
}

void Distributed::run(){

}

void Distributed::stop(){
	if(rank == 0){
		Analysis::output();
	}
	MPI_Finalize();
}
