#include "BlockThrow.h"

BlockThrow::BlockThrow(): BlockParallel(){
	program = "BlockThrow";
}

void BlockThrow::start(){
	BlockParallel::start();
	Range fixRange;
	fixRange.rowStart = 0;
	fixRange.rowEnd = state->patterns.rows()-1;
	fixRange.colStart = state->patterns.columns();
	state->patterns.resize(state->patterns.rows(), state->patterns.columns() + systemSize/2);
	fixRange.colEnd = state->patterns.columns()-1;
	state->patterns.createBuffers();
	state->patterns.fixRange(fixRange);
	state->patterns.matrix = MatrixXd::Constant(state->patterns.rows(),state->patterns.columns(),rank);
}

void BlockThrow::run(){
	state->both = true;
	double error = 0;

	algorithm->setObserver(this);
	cout << "Observer set" << endl;
	algorithm->monteCarlo();
	cout << "Monte Done" << endl;
	averagePatterns();
	averageCoefficients();
	cout << "Average 1" << endl;
	error = algorithm->anneal();
	cout << "Anneal Done" << endl;
	averagePatterns();
	averageCoefficients();
	cout << "Average 2" << endl;
//	if(rank == 0){
//		cout << state->patterns.columns() << '\n' << state->patterns.matrix << "\n\n";
//	}
	Range s = block;
	s.rowEnd = state->patterns.rows()-1;
	s.colEnd -= s.colStart;
	s.rowStart = 0;
	s.colStart = 0;
	state->patterns.shrink(s);
	cout << "Honey I shrunk the matrix" << endl;
//	if(rank == 0){
//		cout << state->patterns.columns() << '\n' << state->patterns.matrix << '\n';
//	}
	gatherPatterns();
	gatherCoefficients();

//	BlockParallel::run();
}

void BlockThrow::stop(){
	BlockParallel::stop();
}
