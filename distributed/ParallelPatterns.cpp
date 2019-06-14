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

//	if(rank == 0){
	oexpression = state->expression;
//	}

	state->dist = to_string(systemSize) + "*1";

	//vector<vector<int>> ranges = state->splitRanges(systemSize);
	Range r = state->getRange(rank);
	//split the coefficients
	state->coefficients.resize(r.rowSize(), state->coefficients.columns());

	//split the expression  
	MatrixXd temp = state->expression.block(r.rowStart, 0, r.rowSize(), state->expression.cols());
	state->expression = temp;

	state->patterns.createBuffers();

	count = state->coefficients.matrix.size();
	disp = r.rowStart*state->coefficients.columns();
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
	int  allCounts[systemSize];
	int  allDispls[systemSize];
	state->coefficients.createBuffers();

	if(rank == 0){
		buffer = new double[oexpression.rows()*state->coefficients.columns()];
	}

	MPI_Gather(&count,1,MPI_INT,&allCounts[0],1,MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Gather(&disp,1,MPI_INT,&allDispls[0],1,MPI_INT, 0, MPI_COMM_WORLD);

	MatrixXd ct = state->coefficients.matrix.transpose();
	copy(ct.data(),ct.data()+ct.size(), state->coefficients.sendBuffer);

	MPI_Gatherv(state->coefficients.sendBuffer, count, MPI_DOUBLE, buffer, allCounts, allDispls, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	state->coefficients.resize(oexpression.rows(),state->coefficients.columns());
	state->expression = oexpression;
	if(rank == 0){
		for(int i = 0; i < oexpression.rows(); i++){
			for(int j = 0; j < state->coefficients.columns(); j++){
				state->coefficients.matrix(i,j) = buffer[j+state->coefficients.columns()*i];
			}
		}
//		Map<MatrixXd> mapper(buffer,oexpression.rows(),state->coefficients.columns());
//		cout << "Map:" << '\n' << mapper << endl;
//		state->coefficients.matrix = mapper;
//		cout << "Final:" << '\n' << state->coefficients.matrix << endl;
		ErrorFunctionRow efRow(state);
		double error = efRow.error();

		cout << "Final Error: " << error/state->expression.size() << endl;
		delete[] buffer;
	}
}

void ParallelPatterns::run(){
	state->both = true;
	double error = 0;

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
	Distributed::stop();
}
