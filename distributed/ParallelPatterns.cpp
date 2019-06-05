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
	MatrixXd temp = state->expression.block(r.rowStart, 0, myRows, state->expression.cols());
	state->expression = temp;

	state->patterns.createBuffers();

	count = state->coefficients.size();
	disp = r.rowStart*state->coefficients.columns;
}

void ParallelPatterns::allAverage(NMFMatrix& mat, MPI_Comm Comm){
	int gSize;
	MPI_Comm_size(Comm,&gSize);
	mat.write(&mat.sendBuffer[0]);
	//all reduce
	MPI_Allreduce(mat.sendBuffer, mat.recvBuffer, mat.size(), MPI_DOUBLE, MPI_SUM, Comm);
	for(int q = 0; q < mat.size(); q++){
		mat.recvBuffer[q] /= gSize;
	}
	mat.read(&mat.recvBuffer[0]);
}

void ParallelPatterns::monteCallback(int iter){
	if(state->both){
		allAverage(state->patterns,MPI_COMM_WORLD);	
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
		allAverage(state->patterns,MPI_COMM_WORLD);
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
	double* sendBuf = new double[state->coefficients.matrix.size()];

	if(rank == 0){
		buffer = new double[oexpression.rows()*state->coefficients.matrix.cols()];
	}

	MPI_Gather(&count,1,MPI_INT,&allCounts[0],1,MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Gather(&disp,1,MPI_INT,&allDispls[0],1,MPI_INT, 0, MPI_COMM_WORLD);

	MatrixXd ct = state->coefficients.matrix.transpose();

	copy(ct.data(),ct.data()+ct.size(), sendBuf);

	MPI_Gatherv(sendBuf, count, MPI_DOUBLE, buffer, allCounts, allDispls, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	state->coefficients.resize(oexpression.rows(),state->coefficients.matrix.cols());
	state->expression = oexpression;
	if(rank == 0){
//		Map<MatrixXd> mapper(buffer,oexpression.rows(),state->coefficients.matrix.cols());
//		state->patterns.matrix = mapper;
		double* nb = buffer;
		MatrixXd temp = MatrixXd::Zero(oexpression.rows(),state->coefficients.matrix.cols());
		for(int i =0; i < temp.rows(); ++i){
			for(int j=0; j < temp.cols(); ++j){
				temp(i,j) = *buffer;
				buffer += 1;
			}
		}
		state->coefficients.matrix = temp;
	//	ErrorFunctionRow efRow(state);
	//	double error = efRow.error();

	//	cout << "Final Error: " << error << endl;
	//	cout << "Patterns: " << endl;
	//	cout << state->patterns.matrix << endl;;
//		delete[] nb;
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
	allAverage(state->patterns,MPI_COMM_WORLD);
	error = algorithm->anneal();
	allAverage(state->patterns,MPI_COMM_WORLD);

	gatherCoefficients();
}

void ParallelPatterns::stop(){
	delete[] sendBuffer;
	delete[] recvBuffer;

	Distributed::stop();
}
