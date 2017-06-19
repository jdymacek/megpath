/*
	Distributed main file
	Matthew Dyer
	Created on 5/31/2017
	Last Modified: 6/5/2017
 */

#include "DistNaive.h"

using namespace std;

DistNaive::DistNaive():Distributed(){}

void DistNaive::start(string filename){
	Distributed::start(filename);
}


void DistNaive::run(){
	int tag = 0;
	MPI_Status status;

	uniform = new UniformPF();

	monteCarlo();
	double formerError = anneal();

	double* recvBuffer = new double[size];

	MPI_Allgather(&formerError,1, MPI_DOUBLE, recvBuffer, 1, MPI_DOUBLE, MPI_COMM_WORLD);

	//find the smallest
	int smallest = -1;
	double minError = state->coefficients.matrix.size()*2;
	for(int i =0; i < size; ++i){
		if(minError > recvBuffer[i]){
			minError = recvBuffer[i];
			smallest = i;
		}
	}


	//if i have the smallest send
	if(smallest == rank){
		cout << hostname << " has the smallest error: " << formerError << endl;
		sendMatrix(state->patterns.matrix,0);
		sendMatrix(state->coefficients.matrix,0);
	}

	if(rank == 0){
		//recieve info
		recvMatrix(state->patterns.matrix,smallest);	
		recvMatrix(state->coefficients.matrix,smallest);
	}
	delete[] recvBuffer;

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
	Distributed::stop();
}
