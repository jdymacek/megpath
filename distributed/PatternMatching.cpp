//Parallel Pattern Matching cpp file
//Matthew Dyer
//Created on : 7/7/2017
//Last Modified 7/7/2017

#include "PatternMatching.h"

PatternMatching::PatternMatching(): Distributed(){
	program = "PatternMatching";
}

void PatternMatching::start(string filename){
	ParallelPatterns::start(filename);
}


void PatternMatching::readMatrix(double* data,MatrixXd& matrix){
	Map<MatrixXd> mapper(data,matrix.rows(),matrix.cols());
	matrix = mapper;
}

void PatternMatching::writeMatrix(double* data,MatrixXd& matrix){
	memcpy(data,matrix.data(),(matrix.size()*sizeof(double)));
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

			MatrixXd myPatterns = state->patterns.matrix;
			sendBuffer[0] = error;

			if(rank == 0)
				writeMatrix(&sendBuffer[1],myPatterns);

			MPI_Bcast(sendBuffer,bufferSize,MPI_DOUBLE,0,MPI_COMM_WORLD);

			if(rank != 0){
				readMatrix(&sendBuffer[1],myPatterns);
				state->patternMatch(myPatterns);
			}
			allAverage();
/*			state->patterns.write(&sendBuffer[1]);

			MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			for(int q = 0; q < bufferSize; q++){
				recvBuffer[q] /= size;
			}
			state->patterns.read(&recvBuffer[1]);
*/
		}

		if(i % state->printRuns == 0){ //for printing
			error = efRow.error();
			cout << hostname << ": " << i << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
		}
	}
	//final reduction
	allAnnealAverage();
/*	state->patterns.write(&sendBuffer[1]);
	MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	state->patterns.read(&recvBuffer[1]);
	state->patterns.matrix /= size;
*/
	cout << hostname << "\tFinal Error: " << efRow.error() << endl;
	cout << hostname << "\tError Histogram: " << efRow.errorDistribution(10) << endl;
	cout << hostname << "\tTotal time: " << watch.formatTime(watch.stop()) << endl;

	delete[] sendBuffer;
	delete[] recvBuffer;

	return efRow.error();
}

double PatternMatching::anneal(){
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
		if(state->both){
			annealStep(state->coefficients,t,&efRow);
			annealStep(state->patterns,t,&efCol);
		}else{
			annealStep(state->coefficients,t,&efRow);
		}

		if(ndx % 1000 == 0){
			error = efRow.error();
			if(state->both == true){
				sendBuffer[0] = error;

				MatrixXd myPatterns = state->patterns.matrix;
				sendBuffer[0] = error;

				if(rank == 0)
					writeMatrix(&sendBuffer[1],myPatterns);

				MPI_Bcast(sendBuffer,bufferSize,MPI_DOUBLE,0,MPI_COMM_WORLD);

				if(rank != 0){
					readMatrix(&sendBuffer[1],myPatterns);
					state->patternMatch(myPatterns);
				}
				allAnnealAverage();
				/*state->patterns.write(&sendBuffer[1]);
				MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
				state->patterns.read(&recvBuffer[1]);
				state->patterns.matrix /= size;
				*/
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

	if(state->both == true){
		allAnnealAverage();
		//final reduction
		/*state->patterns.write(&sendBuffer[1]);
		MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
		state->patterns.read(&recvBuffer[1]);
		state->patterns.matrix /= size;
		*/
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

