/*
	Distributed main file
	Matthew Dyer
	Created on 5/31/2017
	Last Modified: 6/5/2017
 */

#include "DistNaive.h"

using namespace std;

DistNaive::DistNaive(): MonteAnneal(){}

void DistNaive::start(string filename){
	Analysis::start(filename);
    MPI_Init(NULL, NULL);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
}


void DistNaive::run(){
	Stopwatch watch;
	watch.start();

	ProbFunc::generator.seed(time(0));
	int minRank = 0;

	//MPI variables
	int process = 0;
	char hostname[100];
	int tag = 0;
	MPI_Status status;

	int flag = 0;

	//Initialize MPI
	MPI_Comm_size (MPI_COMM_WORLD, &process);
	gethostname(hostname,99);

	uniform = new UniformPF();

	cout << hostname << " got : " << state->analysis << " " << state->MAX_RUNS << " " << state->filename << endl;

	monteCarlo();
	double formerError = anneal();

	MPI_Send(&formerError,sizeof(double),MPI_DOUBLE,0,tag,MPI_COMM_WORLD);
	double minError = formerError;

	if(rank == 0){ //I am the manager
		double err = 0;
		for(int i = 0; i < process; ++i){
			MPI_Recv(&err,1,MPI_DOUBLE,i,tag,MPI_COMM_WORLD,&status);
			if(err < minError){
				minError = err;
				minRank = i;
			}
		}	
		int request = 1;
		if(minRank != 0){
			MPI_Send(&request,1,MPI_INT,minRank,tag,MPI_COMM_WORLD);
		}else{
			cout << "The manager found the smallest error.\n";
			cout << state->patterns.matrix << endl;
		}

		request = 0;
		for(int i = 0; i < process; ++i){
			if(i != minRank){
				MPI_Send(&request,1,MPI_INT,i,tag,MPI_COMM_WORLD);
			}
		}

		if(minRank != 0){
			MPI_Recv(state->patterns.matrix.data(),state->patterns.matrix.rows()*state->patterns.matrix.cols(),MPI_DOUBLE,minRank,tag,MPI_COMM_WORLD,&status);
			MPI_Recv(state->coefficients.matrix.data(),state->coefficients.matrix.rows()*state->coefficients.matrix.cols(),MPI_DOUBLE,minRank,tag,MPI_COMM_WORLD,&status);
			cout << state->patterns.matrix << endl;
		}
		cout << "Minimum error: " << minError << endl;

	}else{ //I am a child
		MPI_Recv(&flag,1,MPI_INT,0,tag,MPI_COMM_WORLD,&status);
		if(flag == 1){
			// I did the best! Send my matrices to the manager
			cout << hostname << " found the smallest error.\n";
			cout << hostname <<" matrix: \n" << state->patterns.matrix << endl;
			MPI_Send(state->patterns.matrix.data(),state->patterns.matrix.rows()*state->patterns.matrix.cols(),MPI_DOUBLE,0,tag,MPI_COMM_WORLD);
			MPI_Send(state->coefficients.matrix.data(),state->coefficients.matrix.rows()*state->coefficients.matrix.cols(),MPI_DOUBLE,0,tag,MPI_COMM_WORLD);
		}
	}
    if(rank == 0){
        cout << "Total program running time: " << watch.formatTime(watch.stop()) << endl;
    }

}

void DistNaive::stop(){
	//write out the best matrices to files
	if(rank == 0){
		state->patterns.write(state->analysis + "patterns.csv");
		state->coefficients.write(state->analysis + "coefficients.csv");

		ofstream fout;
		fout.open(state->analysis + "expression.txt");
		fout << state->coefficients.matrix*state->patterns.matrix;
		fout.close();
	}

	MPI_Finalize();

}
