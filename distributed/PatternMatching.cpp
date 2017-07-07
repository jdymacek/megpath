//Parallel Pattern Matching cpp file
//Matthew Dyer
//Created on : 7/7/2017
//Last Modified 7/7/2017

#include "PatternMatching.h"

PatternMatching::PatternMatching(): Distributed(){
	program = "PatternMatching";
}

int PatternMatching::findStart(int myRank, int curSize, int numRows){
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

int PatternMatching::findRows(int myRank, int curSize, int numRows){
	int split = numRows/curSize;
	int leftover = numRows%curSize;
	if(myRank < leftover){
		split = split + 1;
	}
	return split;
}

void PatternMatching::start(string filename){
	Distributed::start(filename);
	if(rank == 0){
		oexpression = state->expression;
	}

	//split the coefficients
	int myRows = findRows(rank, size, state->coefficients.rows);
	state->coefficients.resize(myRows, state->coefficients.columns);

	//split the expression	
	startPoint = findStart(rank, size, state->expression.rows());
	myRows = findRows(rank, size, state->expression.rows());
	MatrixXd temp = state->expression.block(startPoint, 0, myRows, state->expression.cols());
	state->expression = temp;
}

double PatternMatching::monteCarlo(){	
	int bufferSize = state->patterns.size()+1;
	double* sendBuffer = new double[bufferSize];
	double* recvBuffer = new double[bufferSize];
	for(int i =0; i < bufferSize; ++i){
		recvBuffer[i] = 0;
	}

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
			if(isnan(error)){
				cout << state->patterns.matrix << endl;
			}

			sendBuffer[0] = error;
			state->patterns.write(&sendBuffer[1]);

			MPI_Bcast(sendBuffer,bufferSize,MPI_DOUBLE,0,MPI_COMM_WORLD);

			MatrixXd myPatterns = state->patterns.matrix;
			state->patterns.read(&sendBuffer[1]);
			state->patternMatch(myPatterns);

			state->patterns.write(&sendBuffer[1]);

			MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			for(int q = 0; q < bufferSize; q++){
				recvBuffer[q] /= size;
			}
			state->patterns.read(&recvBuffer[1]);
		}

		if(i % state->printRuns == 0){ //for printing
			error = efRow.error();
			cout << hostname << ": " << i << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
		}
	}
	//final reduction
	state->patterns.write(&sendBuffer[1]);
	MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	state->patterns.read(&recvBuffer[1]);
	state->patterns.matrix /= size;

	cout << hostname << "\tFinal Error: " << efRow.error() << endl;
	cout << hostname << "\tError Histogram: " << efRow.errorDistribution(10) << endl;
	cout << hostname << "\tTotal time: " << watch.formatTime(watch.stop()) << endl;

	delete[] sendBuffer;
	delete[] recvBuffer;

	return efRow.error();
}

double PatternMatching::anneal(bool both){
	int bufferSize = state->patterns.size()+1;
	double* sendBuffer = new double[bufferSize];
	double* recvBuffer = new double[bufferSize];

	Stopwatch watch;
	int ndx = 0;
	double t = 0.5;
	double error = 0;
	watch.start();

	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);

	double formerError = 2*state->expression.rows()*state->expression.cols();
	bool running = true;
	while(running && ndx < 2*state->MAX_RUNS){
		if(both == true){
			annealStep(state->coefficients,t,&efRow);
			annealStep(state->patterns,t,&efCol);
		}else{
			annealStep(state->coefficients,t,&efRow);
		}

		if(ndx % 1000 == 0){
			error = efRow.error();
			if(both == true){
				sendBuffer[0] = error;
				state->patterns.write(&sendBuffer[1]);

				MPI_Bcast(sendBuffer,bufferSize,MPI_DOUBLE,0,MPI_COMM_WORLD);

				MatrixXd myPatterns = state->patterns.matrix;
				state->patterns.read(&sendBuffer[1]);
				state->patternMatch(myPatterns);

				state->patterns.write(&sendBuffer[1]);

				MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
				state->patterns.read(&recvBuffer[1]);
				state->patterns.matrix /= size;
			}
			formerError = error;
		}

		if(ndx % state->printRuns == 0){ //for printing
			error = efRow.error();
			cout << hostname << ": " << ndx << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
		}

		ndx++;
		t *= 0.99975;
	}

	if(both == true){
		//final reduction
		state->patterns.write(&sendBuffer[1]);
		MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
		state->patterns.read(&recvBuffer[1]);
	}

	cout << hostname << " Anneal Final Error: " << efRow.error() << endl;
	cout << "Anneal Error Histogram: " << efRow.errorDistribution(10) << endl;
	cout << "Anneal Total time: " << watch.formatTime(watch.stop()) << endl;

	delete[] sendBuffer;
	delete[] recvBuffer;

	return efRow.error();
}

void PatternMatching::gatherCoefficients(){
	ParallelPatterns::gatherCoefficients();
}

void PatternMatching::run(){
	ParallelPatterns::run();
}

void PatternMatching::stop(){
	Distributed::stop();	
}

