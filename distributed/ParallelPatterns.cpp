//Parallel Patterns main file
//Julian Dymacek
//Matthew Dyer
//Created on : 6/9/2017
//Last Modified 6/12/2017

#include "ParallelPatterns.h"

ParallelPatterns::ParallelPatterns(): MonteAnneal(){}

void ParallelPatterns::start(string filename){
	MonteAnneal::start(filename);
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank != 0){
		MPI_Comm_split(MPI_COMM_WORLD, 0, rank, &workers);
	}else{
		MPI_Comm_split(MPI_COMM_WORLD, 1, rank, &workers);
	}
		
	MPI_Comm_rank(workers, &workers_rank);
	MPI_Comm_size(workers, &workers_size);

	nums = new int[workers_size];

	char hostbuff[100];
	gethostname(hostbuff,99);
	hostname = string(hostbuff);

}

double ParallelPatterns::monteCarlo(){
	int tag = 0;
	int flag = 0;
	MPI_Status status;
	MPI_Request req;
	int bufferSize = state->patterns.size()+1;
	double* buffer = new double[bufferSize];

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
		MPI_Allgather(&workers_rank, 1, MPI_INT, nums, 1, MPI_INT, workers);
			/*error = efRow.error();
			buffer[0] = error;
			state->patterns.write(&buffer[1]);	
			int randProcess = (rand()%(size-1))+1;
			MPI_Isend(buffer,bufferSize,MPI_DOUBLE,randProcess,tag,MPI_COMM_WORLD,&req);
			MPI_Iprobe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&flag,&status);
			if(flag == 1){
				int source = status.MPI_SOURCE;
				MPI_Recv(buffer,bufferSize,MPI_DOUBLE,source,tag,MPI_COMM_WORLD,&status);
				if(buffer[0] < error){
					state->patterns.read(&buffer[1]);
				}
				cout << hostname << " switched patterns -- old error: " << error << endl;
				error = efRow.error();
			}*/
			cout << hostname << ": " << i << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
		}
	}

	cout << hostname << "\tFinal Error: " << efRow.error() << endl;
	cout << hostname << "\tError Histogram: " << efRow.errorDistribution(10) << endl;
	cout << hostname << "\tTotal time: " << watch.formatTime(watch.stop()) << endl;

	delete buffer;

	return error;
}

void ParallelPatterns::run(){
	double error = 0;
	double* allError;

	//MPI variables
	int tag  = 0;
	MPI_Status status;

	if(rank == 0){
		allError = new double[size];
	}else{
		monteCarlo();
		error = anneal();
		cout << hostname << " has " << error << " error" << endl;
	}

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

	if(rank != 0 && workers_rank == 0){
		cout << ">-----[ ";
		for(int i = 0; i < size; ++i){
			cout << nums[i] << " ";
		}
		cout << "]-----<" << endl;
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
	
	delete nums;

	MPI_Comm_free(&workers);
	MPI_Finalize();

}
