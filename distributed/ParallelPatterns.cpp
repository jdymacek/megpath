//Parallel Patterns main file
//Julian Dymacek
//Matthew Dyer
//Created on : 6/9/2017
//Last Modified 5/30/2019

#include "ParallelPatterns.h"

ParallelPatterns::ParallelPatterns(): Distributed(){
	program = "ParallelPatterns";
}

void ParallelPatterns::start(){
	Distributed::start();

	//if(rank == 0){
	oexpression = state->expression;
	//}

	state->dist = to_string(size) + "*1";

	//vector<vector<int>> ranges = state->splitRanges(size);
	Range r = state->getRange(rank);
	//split the coefficients
	int myRows = r.rowEnd - r.rowStart;
	state->coefficients.resize(myRows, state->coefficients.columns);
	//split the expression  

	//change to range startRow
	startPoint = r.rowStart;

	//replace with the 4 Range coordinates
	MatrixXd temp = state->expression.block(startPoint, 0, myRows, state->expression.cols());
	state->expression = temp;

	bufferSize = state->patterns.size();
	sendBuffer = new double[bufferSize];
	recvBuffer = new double[bufferSize];

}

void ParallelPatterns::allAverage(){
	state->patterns.write(&sendBuffer[0]);
	//all reduce
	MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	for(int q = 0; q < bufferSize; q++){
		recvBuffer[q] /= size;
	}
	state->patterns.read(&recvBuffer[0]);
}


void ParallelPatterns::monteCallback(int iter){
	if(state->both){
		allAverage();	
	}
}

void ParallelPatterns::montePrintCallback(int iter){
	ErrorFunctionRow ef(state);
	cout << "montecarlo " << hostname << ": " << iter << "\t Error = " << ef.error() << endl;
}

void ParallelPatterns::annealPrintCallback(int iter){
	ErrorFunctionRow ef(state);

	cout << "anneal " << hostname << " " << rank << ": " << iter << "\t Error = " << ef.error() << endl;
}

bool ParallelPatterns::annealCallback(int iter){
	if(state->both){
		if(iter > state->MAX_RUNS*state->annealCutOff)
			state->both = false;
		allAverage();
	}
	return true;
}


void ParallelPatterns::monteFinalCallback(){

}

void ParallelPatterns::annealFinalCallback(){

}

void ParallelPatterns::gatherCoefficients(){
	double* buffer = NULL;
	int  allCounts[size];
	int  allDispls[size];
	double* sendBuf = new double[state->coefficients.matrix.cols()*state->coefficients.matrix.rows()];

	if(rank == 0){
		buffer = new double[oexpression.rows()*state->coefficients.matrix.cols()];
	}

	int send = state->coefficients.matrix.size();
	MPI_Gather(&send,1,MPI_INT,&allCounts[0],1,MPI_INT, 0, MPI_COMM_WORLD);

	send = state->coefficients.matrix.cols()*startPoint;
	MPI_Gather(&send,1,MPI_INT,&allDispls[0],1,MPI_INT, 0, MPI_COMM_WORLD);

	MatrixXd ct = state->coefficients.matrix.transpose();

	copy(ct.data(),ct.data()+ct.size(), sendBuf);

	MPI_Gatherv(sendBuf,ct.size(),MPI_DOUBLE,
			buffer, allCounts, allDispls, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	state->coefficients.resize(oexpression.rows(),state->coefficients.matrix.cols());
	state->expression = oexpression;
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
		ErrorFunctionRow efRow(state);
		double error = efRow.error();

		cout << "Final Error: " << error << endl;
		cout << "Patterns: " << endl;
		cout << state->patterns.matrix << endl;;
		delete[] nb;
	}
	delete[] sendBuf;
}



void ParallelPatterns::run(){
	state->both = true;
	double error = 0;

	for(int i =0; i < bufferSize; ++i){
		recvBuffer[i] = 0;
	}

	//should we set observer in start?
	algorithm->setObserver(this);
	//end set observer
	algorithm->monteCarlo();
	allAverage();
	error = algorithm->anneal();
	allAverage();

	gatherCoefficients();
}

void ParallelPatterns::stop(){
	delete[] sendBuffer;
	delete[] recvBuffer;

	Distributed::stop();
}
