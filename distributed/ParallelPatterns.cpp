//Parallel Patterns main file
//Julian Dymacek
//Matthew Dyer
//Created on : 6/9/2017
//Last Modified 6/13/2017

#include "ParallelPatterns.h"

ParallelPatterns::ParallelPatterns(): Distributed(){
	program = "ParallelPatterns";
}

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

void ParallelPatterns::allAnnealAverage(){
    state->patterns.write(&sendBuffer[0]);
    //all reduce
    MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    state->patterns.read(&recvBuffer[0]);
    state->patterns.matrix /= size;
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


void ParallelPatterns::monteCallback(double error){
	if(state->both){
		allAverage();	
	}
	if(iterations % state->printRuns == 0){ //for switching
		cout << hostname << ": " << i << "\t Error = " << error << endl;
	}

	/*cout << hostname << "\tFinal Error: " << efRow.error() << endl;
	cout << hostname << "\tError Histogram: " << efRow.errorDistribution(10) << endl;
	cout << hostname << "\tTotal time: " << watch.formatTime(watch.stop()) << endl;
	*/
}


void ParallelPatterns::annealCallback(double error){
	if(state->both){
		allAnnealAverage();
	}
	cout << hostname << ": " << iterations << "\t Error = " << error << endl;
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
        double error = efRow.error();

        cout << "Final Error: " << error << endl;
        cout << "Patterns: " << endl;
        cout << state->patterns.matrix << endl;;
        delete[] nb;
    }
	delete[] sendBuf;
}

void ParallelPatterns::run(){
	double error = 0;

    bufferSize = state->patterns.size();
    sendBuffer = new double[bufferSize];
	recvBuffer = new double[bufferSize];
    for(int i =0; i < bufferSize; ++i){
        recvBuffer[i] = 0;
    }

	algorithm->setObserver(this);
	algorithm->monteCarlo();
	allAnnealAverage();
	error = algorithm->anneal();
	allAnnealAverage();
	state->both = false;
	error = algorithm->anneal();

	delete[] sendBuffer;
    delete[] recvBuffer;

	gatherCoefficients();
}

void ParallelPatterns::stop(){
	Distributed::stop();
}
