
#include "FuncThrow.h"

FuncThrow::FuncThrow(): Distributed(){
	program = "FuncThrow";
}

void FuncThrow::start(string filename){
	Distributed::start(filename);
	bufferSize = state->patterns.size()+1;
	buffer = new double[bufferSize];
	recvBuffer = new double[bufferSize];
	//needs to be changed
	srand(time(0));
}


void FuncThrow::monteCallback(int iter){
	int testFlag = 0;
	int flag = 0;
	int tag =0;
	MPI_Status status;
	MPI_Request req = MPI_REQUEST_NULL;
	ErrorFunctionRow efRow(state);
	double error = 0;

	error = efRow.error();
	buffer[0] = error;
	state->patterns.write(&buffer[1]);	
			
	MPI_Test(&req,&testFlag,MPI_STATUS_IGNORE);
		if(testFlag == 1){
			testFlag = 0;
			int randProcess = (rand()%(size-1))+1;
			MPI_Isend(buffer,bufferSize,MPI_DOUBLE,randProcess,tag,MPI_COMM_WORLD,&req);
		}
	MPI_Iprobe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&flag,&status);
		if(flag == 1){
			flag = 0;
			int source = status.MPI_SOURCE;
			MPI_Recv(recvBuffer,bufferSize,MPI_DOUBLE,source,tag,MPI_COMM_WORLD,&status);
			if(recvBuffer[0] < error){
				state->patterns.read(&recvBuffer[1]);
			}
		}

		/*if(i % state->printRuns == 0){ //for printing
			error = efRow.error();
			cout << hostname << ": " << i << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
		}*/

}

void FuncThrow::finished(){
    int testFlag = 0;
    int tag =0;
    MPI_Status status;
    MPI_Request req = MPI_REQUEST_NULL;	

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Test(&req,&testFlag,MPI_STATUS_IGNORE);
	if(testFlag == 0){
		MPI_Cancel(&req);
		MPI_Request_free(&req);
	}

	/*cout << hostname << "\tFinal Error: " << efRow.error() << endl;
	cout << hostname << "\tError Histogram: " << efRow.errorDistribution(10) << endl;
	cout << hostname << "\tTotal time: " << watch.formatTime(watch.stop()) << endl;
*/
	//return error;
}

void FuncThrow::run(){
	double error = 0;
	double* allError;

	//MPI variables
	int tag = 0;
	MPI_Status status;

	if(rank == 0){
		cout << "Rank 0 is " << hostname << endl;
	}
	//set callback
	algorithm->setObserver(this);

	algorithm->monteCarlo();
	finished();



	double formerError = algorithm->anneal();
	//finished?

	double* getBuffer = new double[size];

	MPI_Allgather(&formerError,1, MPI_DOUBLE, getBuffer, 1, MPI_DOUBLE, MPI_COMM_WORLD);

	//find the smallest
	int smallest = -1;
	double minError = state->coefficients.matrix.size()*2;
	for(int i =0; i < size; ++i){
		if(minError > getBuffer[i]){
			minError = getBuffer[i];
			smallest = i;
		}
	}

	//if i have the smallest send
	if(smallest == rank){
		cout << hostname << " has the smallest error: " << formerError << endl;
		sendMatrix(state->patterns.matrix,0);
		sendMatrix(state->coefficients.matrix,0);
	}

	if(rank == 0){
		//recieve info
		recvMatrix(state->patterns.matrix,smallest);	
		recvMatrix(state->coefficients.matrix,smallest);
	}
	delete[] getBuffer;
}

void FuncThrow::stop(){
	delete[] buffer;
	delete[] recvBuffer;
	Distributed::stop();
}
