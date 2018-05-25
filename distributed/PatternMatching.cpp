//Parallel Pattern Matching cpp file
//Matthew Dyer
//Created on : 7/7/2017
//Last Modified 7/7/2017

#include "PatternMatching.h"

PatternMatching::PatternMatching(): ParallelPatterns(){
	program = "PatternMatching";
}

/*void PatternMatching::start(string filename){
	ParallelPatterns::start(filename);
}*/


void PatternMatching::readMatrix(double* data,MatrixXd& matrix){
	Map<MatrixXd> mapper(data,matrix.rows(),matrix.cols());
	matrix = mapper;
}

void PatternMatching::writeMatrix(double* data,MatrixXd& matrix){
	memcpy(data,matrix.data(),(matrix.size()*sizeof(double)));
}

void PatternMatching::matchPatterns(){
    MatrixXd myPatterns = state->patterns.matrix;
    if(rank == 0){
        writeMatrix(&sendBuffer[0],myPatterns);
    }

    MPI_Bcast(sendBuffer,bufferSize,MPI_DOUBLE,0,MPI_COMM_WORLD);

    if(rank != 0){
        readMatrix(&sendBuffer[0],myPatterns);
        state->patternMatch(myPatterns);
    }
}

void PatternMatching::monteCallback(double error){
	matchPatterns();
	allAverage();
}

void PatternMatching::annealCallback(double error){
	if(state->both == true){
		matchPatterns();
		allAnnealAverage();
	}
}

//for printing
//cout << hostname << ": " << ndx << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;


/*void PatternMatching::gatherCoefficients(){
	ParallelPatterns::gatherCoefficients();
}*/

/*void PatternMatching::run(){
	ParallelPatterns::run();
}*/

/*void PatternMatching::stop(){
	Distributed::stop();	
}*/

