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
	state->expression.conservativeResize(state->coefficients.rows(),state->patterns.columns());

	int smallCol = systemSize;
	for(auto c: cSets){
		int cSize;
		MPI_Comm_size(c.comm,&cSize);
		cout << rank << '\t' << cSize << endl;
		if(cSize < smallCol){
			smallCol = cSize;
		}
	}
	shareSets.resize(smallCol);

	MPI_Group worldGroup;
	MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);
	for(int i = 0; i < smallCol; i++){
		int nSize = 0;
		for(int j = 0; j < systemSize; j++){
			if(j%smallCol == i){
				nSize++;
			}
		}
		int NGA[nSize];
		int k = 0;
		for(int j = 0; j < systemSize; j++){
			if(j%smallCol == i){
				NGA[k] = j;
				k++;
			}
		}
		cout << rank << ' ' << i << ":\n";
		for(int j = 0; j < nSize; j++){
			cout << NGA[j] << ' ';
		}
		cout << endl;
		MPI_Group_incl(worldGroup,nSize,&NGA[0],&shareSets[i].group);
		MPI_Comm_create(MPI_COMM_WORLD,shareSets[i].group,&shareSets[i].comm);
	}
}

void BlockThrow::run(){
	state->both = true;
	double error = 0;

	algorithm->setObserver(this);
	algorithm->monteCarlo();
	averagePatterns();
	averageCoefficients();
	error = algorithm->anneal();
	averagePatterns();
	averageCoefficients();
	//	if(rank == 0){
	//		cout << state->patterns.columns() << '\n' << state->patterns.matrix << "\n\n";
	//	}
	Range s = block;
	s.rowEnd = state->patterns.rows()-1;
	s.colEnd -= s.colStart;
	s.rowStart = 0;
	s.colStart = 0;
	state->patterns.shrink(s);
	//	if(rank == 0){
	//		cout << state->patterns.columns() << '\n' << state->patterns.matrix << '\n';
	//	}
	gatherPatterns();
	gatherCoefficients();

	//	BlockParallel::run();
}

void BlockThrow::monteCallback(int iter){
//	if(rank == 1){
//		cout << state->patterns.matrix << '\n' << iter << '\n';
//	}
	for(int i = 0; i < state->patterns.rows(); i++){
		for(int j = block.colSize(); j < state->patterns.columns(); j++){
			state->patterns.functions(i,j)->addObservation(iter*rank/state->interruptRuns);
			state->patterns.matrix(i,j) = iter*rank/state->interruptRuns;
		}
	}
	if(state->both && iter/state->interruptRuns%2 == 0){
		averagePatterns();
	}else{
		averageCoefficients();
	}
//	if(rank == 1){
//		cout << state->patterns.matrix << '\n';
//	}
}

bool BlockThrow::annealCallback(int iter){
	if(state->both && iter/state->interruptRuns%2 == 0){
		averagePatterns();
		if(iter > state->MAX_RUNS*state->annealCutOff)
			state->both = false;
	}else{
		averageCoefficients();
	}
	return true;
}

void BlockThrow::stop(){
	BlockParallel::stop();
}
