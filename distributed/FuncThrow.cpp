
#include "FuncThrow.h"

FuncThrow::FuncThrow(): Distributed(){
	program = "FuncThrow";
}

void FuncThrow::start(string filename){
	Distributed::start(filename);
	bufferSize = state->patterns.size()+1;
	buffer = new double[bufferSize];
	recvBuffer = new double[bufferSize];
	srand(time(0));
}

double FuncThrow::monteCarlo(){
	int tag = 0;
	int flag = 0;
	MPI_Status status;
	MPI_Request req;

	double error = 0;
	Stopwatch watch;
	watch.start();

	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);

	//For each spot take a gamble and record outcome
	for(int i =0; i < state->MAX_RUNS; i++){
		monteCarloStep(state->patterns,&efCol);
		monteCarloStep(state->coefficients,&efRow);

		if(i%1000 == 0){ //for switching
			error = efRow.error();
			buffer[0] = error;
			state->patterns.write(&buffer[1]);	
			int randProcess = (rand()%(size-1))+1;
			//MPI_Test
			//check if flag is set
				MPI_Isend(buffer,bufferSize,MPI_DOUBLE,randProcess,tag,MPI_COMM_WORLD,&req);
			//close if
			MPI_Iprobe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&flag,&status);
			if(flag == 1){
				int source = status.MPI_SOURCE;
				MPI_Recv(recvBuffer,bufferSize,MPI_DOUBLE,source,tag,MPI_COMM_WORLD,&status);
				if(recvBuffer[0] < error){
					state->patterns.read(&recvBuffer[1]);
				}
			//	cout << hostname << " switched patterns -- old error: " << error << endl;
				error = efRow.error();
			}
		}

		if(i % state->printRuns == 0){ //for printing
			error = efRow.error();
			cout << hostname << ": " << i << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
		}
	}

	//Barrier
	//test for outstanding send
		//if so cancel it and free it

	cout << hostname << "\tFinal Error: " << efRow.error() << endl;
	cout << hostname << "\tError Histogram: " << efRow.errorDistribution(10) << endl;
	cout << hostname << "\tTotal time: " << watch.formatTime(watch.stop()) << endl;

	return error;
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

	monteCarlo();
	double formerError = anneal();

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
	//write out the best matrices to files
	if(rank == 0){
		state->patterns.write(state->analysis + "patterns.csv");
		state->coefficients.write(state->analysis + "coefficients.csv");

		ofstream fout;
		fout.open(state->analysis + "expression.txt");
		fout << state->coefficients.matrix*state->patterns.matrix;
		fout.close();
	}

	delete[] buffer;
	delete[] recvBuffer;

	MPI_Finalize();

}
