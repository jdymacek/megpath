//Block Parallel file
//Julian Dymacek
//Justin Moore
//Created: 5/30/2019
//Last Modified: 5/30/2019

#include "BlockParallel.h"
void BlockParallel::start(){
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
