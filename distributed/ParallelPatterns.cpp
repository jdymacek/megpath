//Parallel Patterns main file
//Julian Dymacek
//Matthew Dyer
//Created on : 6/9/2017
//Last Modified 6/13/2017

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

	if(rank == 0){
		oexpression = state->expression;
	}

	//split the coefficients
	int myRows = findRows(rank, size, state->coefficients.rows);
	state->coefficients.resize(myRows, state->coefficients.columns);
/*	for(int y = 0; y < state->coefficients.matrix.rows(); ++y){
	    for(int x = 0; x < state->coefficients.matrix.cols(); ++x){
			state->coefficients.matrix(y,x) = 100*(x+1)+rank;
		}
	}*/


	//cout << hostname << " coefficients:" << endl;
	//cout << state->coefficients.matrix << endl;

	//split the expression	
	startPoint = findStart(rank, size, state->expression.rows());
	myRows = findRows(rank, size, state->expression.rows());
	MatrixXd temp = state->expression.block(startPoint, 0, myRows, state->expression.cols());
	state->expression = temp;

	//cout << hostname << " expression:" << endl;
	//cout << state->expression << endl;
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

	cout << hostname << "\tFinal Error: " << efRow.error() << endl;
	cout << hostname << "\tError Histogram: " << efRow.errorDistribution(10) << endl;
	cout << hostname << "\tTotal time: " << watch.formatTime(watch.stop()) << endl;

	delete sendBuffer;
	delete recvBuffer;

	return error;
}

double ParallelPatterns::anneal(){
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
		annealStep(state->coefficients,t,&efRow);
		annealStep(state->patterns,t,&efCol);
		if(ndx % 1000 == 0){
			double error = efRow.error();
			sendBuffer[0] = error;
			state->patterns.write(&sendBuffer[1]);
			//all reduce
			MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
			state->patterns.read(&recvBuffer[1]);
			state->patterns.matrix /= size;

			cout << hostname << " " << ndx << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
			//if(abs(formerError - error) < 0.005 && error < formerError)
			//	running = false;
			formerError = error;
		}
		ndx++;
		t *= 0.99975;
	}
	//final reduction
	state->patterns.write(&sendBuffer[1]);
	MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	state->patterns.read(&recvBuffer[1]);
	state->patterns.matrix /= size;

	cout << hostname << " Anneal Final Error: " << efRow.error() << endl;
	cout << "Anneal Error Histogram: " << efRow.errorDistribution(10) << endl;
	cout << "Anneal Total time: " << watch.formatTime(watch.stop()) << endl;

	delete sendBuffer;
	delete recvBuffer;

	return efRow.error();
}

double ParallelPatterns::annealAgain(){
	Stopwatch watch;
	int ndx = 0;
	double t = 0.5;
	watch.start();

   ErrorFunctionRow efRow(state);
   ErrorFunctionCol efCol(state);

	double formerError = 2*state->expression.rows()*state->expression.cols();
	bool running = true;
	while(running && ndx < state->MAX_RUNS){
		annealStep(state->coefficients,t,&efRow);
		//annealStep(state->patterns,t,&efCol);
		if(ndx % 1000 == 0){
			double error = efRow.error();
			cout << hostname << " " << ndx << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
			//if(abs(formerError - error) < 0.005 && error < formerError)
			//	running = false;
			formerError = error;
		}
		ndx++;
		t *= 0.99975;
	}
	cout << hostname << " Final Error: " << efRow.error() << endl;
	cout << hostname << " Error Histogram: " << efRow.errorDistribution(10) << endl;
	cout << hostname << " Total time: " << watch.formatTime(watch.stop()) << endl;

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
	error = anneal();
	annealAgain();

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

	memcpy(sendBuf,ct.data(),sizeof(double)*ct.size());

	MPI_Gatherv(sendBuf,ct.size(),MPI_DOUBLE,
			buffer, allCounts, allDispls, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	cout << hostname << " entering rank 0 area" << endl;

	if(rank == 0){
		
	
		MatrixXd temp = MatrixXd::Zero(oexpression.rows(),state->coefficients.matrix.cols());
		for(int i =0; i < temp.rows(); ++i){
			for(int j=0; j < temp.cols(); ++j){
				temp(i,j) = *buffer;
//				cout << *buffer << " ";
				buffer += 1;
			}
//			cout << endl;
		}
//		cout << "temp is fine" << endl;


		state->coefficients.matrix = temp;


		state->coefficients.rows = state->coefficients.matrix.rows();
		state->coefficients.columns = state->coefficients.matrix.cols();
//		cout << "Coefficients:" << endl;
//		cout << state->coefficients.matrix << endl;

		state->expression = oexpression;
		ErrorFunctionRow efRow(state);
		error = efRow.error();
		cout << "Final Error: " << error << endl;
		cout << "Patterns: " << endl;
		cout << state->patterns.matrix << endl;;
//		delete buffer;
	}
	delete sendBuf;

}

void ParallelPatterns::stop(){
	//write out the best matrices to files
/*	if(rank == 0){
		state->patterns.write(state->analysis + "patterns.csv");
		state->coefficients.write(state->analysis + "coefficients.csv");

		ofstream fout;
		fout.open(state->analysis + "expression.txt");
		fout << state->coefficients.matrix*state->patterns.matrix;
		fout.close();
	}
*/	
	MPI_Finalize();

}
