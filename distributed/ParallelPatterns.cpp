//Parallel Patterns main file
//Julian Dymacek
//Matthew Dyer
//Created on : 6/9/2017
//Last Modified 6/13/2017

#include "ParallelPatterns.h"

ParallelPatterns::ParallelPatterns(): Distributed(){}

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

double ParallelPatterns::monteCarlo(){
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

			MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			for(int q = 0; q < bufferSize; q++){
				recvBuffer[q] /= size;
			}

			state->patterns.read(&recvBuffer[1]);
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

	return error;
}

double ParallelPatterns::anneal(bool both){
	int bufferSize = state->patterns.size()+1;
	double* sendBuffer = new double[bufferSize];
	double* recvBuffer = new double[bufferSize];

	Stopwatch watch;
	int ndx = 0;
	double t = 0.5;
	watch.start();

	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);

	double formerError = 2*state->expression.rows()*state->expression.cols();
	bool running = true;
	while(running && ndx < state->MAX_RUNS){
		if(both == true){
			annealStep(state->coefficients,t,&efRow);
			annealStep(state->patterns,t,&efCol);
		}else{
			annealStep(state->coefficients,t,&efRow);
		}

		if(ndx % 1000 == 0){
			double error = efRow.error();
			if(both == true){
				sendBuffer[0] = error;
				state->patterns.write(&sendBuffer[1]);
				//all reduce
				MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
				state->patterns.read(&recvBuffer[1]);
				state->patterns.matrix /= size;
			}
			cout << hostname << " " << ndx << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
			formerError = error;
		}

		ndx++;
		t *= 0.99975;
	}
	if(both == true){
		//final reduction
		state->patterns.write(&sendBuffer[1]);
		MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
		state->patterns.read(&recvBuffer[1]);
		state->patterns.matrix /= size;
	}

	cout << hostname << " Anneal Final Error: " << efRow.error() << endl;
	cout << "Anneal Error Histogram: " << efRow.errorDistribution(10) << endl;
	cout << "Anneal Total time: " << watch.formatTime(watch.stop()) << endl;

	delete[] sendBuffer;
	delete[] recvBuffer;

	return efRow.error();
}

void ParallelPatterns::run(){
	double error = 0;
	int  allCounts[size];
	int  allDispls[size];

	//MPI variables
	int tag  = 0;
	double* buffer = NULL;
	double* sendBuf = new double[state->coefficients.matrix.cols()*state->coefficients.matrix.rows()];
	MPI_Status status;
	if(rank == 0){
		buffer = new double[oexpression.rows()*state->coefficients.matrix.cols()];
	}

	monteCarlo();
	error = anneal(true);
	anneal(false);

	int send = state->coefficients.matrix.size();
	MPI_Gather(&send,1,MPI_INT,&allCounts[0],1,MPI_INT, 0, MPI_COMM_WORLD);

	send = state->coefficients.matrix.cols()*startPoint;
	MPI_Gather(&send,1,MPI_INT,&allDispls[0],1,MPI_INT, 0, MPI_COMM_WORLD);

	if(rank == 0){
		for(int i = 0; i < size; ++i){
			cout << i << "\t" <<  allCounts[i] << "\t" << allDispls[i] << endl;
		}
		cout << oexpression.rows()*state->coefficients.matrix.cols() << endl;
	}	

	MatrixXd ct = state->coefficients.matrix.transpose();

	copy(ct.data(),ct.data()+ct.size(), sendBuf);

	MPI_Gatherv(sendBuf,ct.size(),MPI_DOUBLE,
			buffer, allCounts, allDispls, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	cout << hostname << " entering rank 0 area" << endl;

	if(rank == 0){
		double* nb = buffer;
		MatrixXd temp = MatrixXd::Zero(oexpression.rows(),state->coefficients.matrix.cols());
		for(int i =0; i < temp.rows(); ++i){
			for(int j=0; j < temp.cols(); ++j){
				temp(i,j) = *buffer;
				buffer += 1;
			}
		}

		state->coefficients.matrix = temp;
		state->coefficients.rows = state->coefficients.matrix.rows();
		state->coefficients.columns = state->coefficients.matrix.cols();
		state->expression = oexpression;
		ErrorFunctionRow efRow(state);
		error = efRow.error();

		cout << "Final Error: " << error << endl;
		cout << "Patterns: " << endl;
		cout << state->patterns.matrix << endl;;
		delete[] nb;
	}

	delete[] sendBuf;

}

void ParallelPatterns::stop(){
	Distributed::stop();
}
