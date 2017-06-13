//Parallel Patterns main file
//Julian Dymacek
//Matthew Dyer
//Created on : 6/9/2017
//Last Modified 6/12/2017

#include "ParallelPatterns.h"

ParallelPatterns::ParallelPatterns(): MonteAnneal(){}

int ParallelPatterns::findStart(int myRank, int curSize, int numRows){
	int startRow = 0;
	int split = numRows/curSize;
	int leftover = numRows%curSize;
	if(myRank < leftover){
		split = split + 1;
		startRow = myRank*split;
	}else{
		startRow = numRows - (curSize - myRank) * split;
	}
	return startRow;
}

int ParallelPatterns::findRows(int myRank, int curSize, int numRows){
	int split = numRows/curSize;
	int leftover = numRows%curSize;
	if(myRank < leftover){
		split = split + 1;
	}
	return split;
}

void ParallelPatterns::start(string filename){
	MonteAnneal::start(filename);
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	char hostbuff[100];
	gethostname(hostbuff,99);
	hostname = string(hostbuff);

	//split the coefficients
	int myRows = findRows(rank, size, state->coefficients.rows);
	state->coefficients.resize(myRows, state->coefficients.columns);
	cout << hostname << " coefficients:" << endl;
	cout << state->coefficients.matrix << endl;

	//split the expression	
	int start = findStart(rank, size, state->expression.rows());
	myRows = findRows(rank, size, state->expression.rows());
	MatrixXd temp = state->expression.block(start, 0, myRows, state->expression.cols());
	state->expression = temp;

	cout << hostname << " expression:" << endl;
	cout << state->expression << endl;
}

double ParallelPatterns::monteCarlo(){
	int tag = 0;
	int flag = 0;
	MPI_Status status;
	MPI_Request req;
	int bufferSize = state->patterns.size()+1;
	double* sendBuffer = new double[bufferSize];
	double* recvBuffer = new double[bufferSize];

	double error = 0;
	Stopwatch watch;
	watch.start();


	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);

	//For each spot take a gamble and record outcome
	for(int i =0; i < state->MAX_RUNS; i++){
		monteCarloStep(state->patterns,&efCol);
		monteCarloStep(state->coefficients,&efRow);

		if(i%1000 == 0){
			error = efRow.error();
			sendBuffer[0] = error;
			cout << hostname << " " <<  i  << " before divide:" << state->patterns.matrix << endl;
			state->patterns.write(&sendBuffer[1]);
			cout << hostname << " " <<  i  << " send error:" << sendBuffer[0] << endl;
			//all reduce
			MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			state->patterns.read(recvBuffer);
			cout << hostname << " " <<  i  << " recieve error:" << recvBuffer[0] << endl;
		
			state->patterns.matrix /= size;
			cout << hostname << " " <<  i  << " after divide:" << state->patterns.matrix << endl;

			cout << hostname << ": " << i << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
		}

	}

	cout << hostname << "\tFinal Error: " << efRow.error() << endl;
	cout << hostname << "\tError Histogram: " << efRow.errorDistribution(10) << endl;
	cout << hostname << "\tTotal time: " << watch.formatTime(watch.stop()) << endl;

	delete sendBuffer;
	delete recvBuffer;

	return error;
}

void ParallelPatterns::run(){
	double error = 0;
	double* allError;

	//MPI variables
	int tag  = 0;
	MPI_Status status;

	if(rank == 0)
		allError = new double[size];

	monteCarlo();
	error = anneal();
	cout << hostname << " has " << error << " error" << endl;

	MPI_Gather(&error, 1, MPI_DOUBLE, allError, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	int minRank = 0;
	if(rank == 0){
		error = state->expression.rows()*state->expression.cols();
		for(int i =1;i < size; ++i){
			if(error >= allError[i]){
				error = allError[i];
				minRank = i;
			}
		}
	}

	MPI_Bcast(&minRank, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if(rank == 0){
		MPI_Recv(state->patterns.matrix.data(),
				state->patterns.matrix.rows()*state->patterns.matrix.cols(),
				MPI_DOUBLE,minRank,tag,MPI_COMM_WORLD,&status);
		MPI_Recv(state->coefficients.matrix.data(),
				state->coefficients.matrix.rows()*state->coefficients.matrix.cols(),
				MPI_DOUBLE,minRank,tag,MPI_COMM_WORLD,&status);
	}else if(minRank == rank){
		char c = 7;
		cout << c << hostname << " is sending the final data. The minimum error was " << error << "." << endl;
		MPI_Send(state->patterns.matrix.data(),
				state->patterns.matrix.rows()*state->patterns.matrix.cols(),
				MPI_DOUBLE,0,tag,MPI_COMM_WORLD);
		MPI_Send(state->coefficients.matrix.data(),
				state->coefficients.matrix.rows()*state->coefficients.matrix.cols(),
				MPI_DOUBLE,0,tag,MPI_COMM_WORLD);
	}

}

void ParallelPatterns::stop(){
	//write out the best matrices to files
	if(rank == 0){
		state->patterns.write(state->analysis + "patterns.csv");
		state->coefficients.write(state->analysis + "coefficients.csv");

		ofstream fout;
		fout.open(state->analysis + "expression.txt");
		fout << state->coefficients.matrix*state->patterns.matrix;
		fout.close();
	}
	
	MPI_Finalize();

}
