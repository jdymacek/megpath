//Parallel Patterns main file
//Julian Dymacek
//Matthew Dyer
//Created on : 6/9/2017
//Last Modified 6/13/2017

#include "ParallelPatterns.h"

ParallelPatterns::ParallelPatterns(): Distributed(){
	program = "ParallelPatterns";
}

void ParallelPatterns::start(){
	Distributed::start();

    if(rank == 0){
        oexpression = state->expression;
    }

    vector<vector<int>> ranges = state->splitRanges(size);
    //split the coefficients
    int myRows = ranges[rank][3] - ranges[rank][2];
    state->coefficients.resize(myRows, state->coefficients.columns);
	 //split the expression  

    startPoint = ranges[rank][2];

    MatrixXd temp = state->expression.block(startPoint, 0, myRows, state->expression.cols());
    state->expression = temp;

    bufferSize = state->patterns.size();
    sendBuffer = new double[bufferSize];
    recvBuffer = new double[bufferSize];

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


void ParallelPatterns::monteCallback(int iter){
	if(state->both){
		allAverage();	
	}

	/*cout << hostname << "\tFinal Error: " << efRow.error() << endl;
	cout << hostname << "\tError Histogram: " << efRow.errorDistribution(10) << endl;
	cout << hostname << "\tTotal time: " << watch.formatTime(watch.stop()) << endl;
	*/
}

void ParallelPatterns::montePrintCallback(int iter){
	ErrorFunctionRow ef(state);
	cout << "montecarlo " << hostname << ": " << iterations << "\t Error = " << ef.error() << endl;
}

void ParallelPatterns::annealPrintCallback(int iter){
    ErrorFunctionRow ef(state);
	
  cout << "anneal " << hostname << " " << rank << ": " << iterations << "\t Error = " << ef.error() << endl;
}

bool ParallelPatterns::annealCallback(int iter){
	if(state->both){
		if(iter > state->MAX_RUNS*state->annealCutOff)
			state->both = false;
		allAnnealAverage();
	}
	return true;
}


void ParallelPatterns::gatherCoefficients(){
	double* buffer = NULL;
    int  allCounts[size];
    int  allDispls[size];
    double* sendBuf = new double[state->coefficients.matrix.cols()*state->coefficients.matrix.rows()];

    	if(state->debug){
		cout << hostname << " entering gatherCoefficients" << endl;
	}
	
	if(rank == 0){
		if(state->debug){
			cout << hostname << " creating stuff" << endl;
		}
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

    if(state->debug){
	cout << hostname << " all gathered" << endl;
    }

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
	state->both = true;
	double error = 0;
	
    for(int i =0; i < bufferSize; ++i){
        recvBuffer[i] = 0;
    }

	//should we set observer in start?
	algorithm->setObserver(this);
	//end set observer
	algorithm->monteCarlo();
	allAnnealAverage();
	error = algorithm->anneal();
	if(state->debug){
		cout << state->patterns.matrix << endl << endl;
	}	
	allAnnealAverage();

	gatherCoefficients();
}

void ParallelPatterns::stop(){
	delete[] sendBuffer;
    delete[] recvBuffer;

    if(state->debug){
	    cout << "interupt_runs: " << state->interuptRuns << endl;
    }
	Distributed::stop();
}
