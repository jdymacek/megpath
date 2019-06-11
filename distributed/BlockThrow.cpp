#include "BlockThrow.h"
#include "time.h"
#include <set>

BlockThrow::BlockThrow(): BlockParallel(){
	program = "BlockThrow";
}

void BlockThrow::start(){
	srand(time(0));
	BlockParallel::start();

	Range fixRange;
	fixRange.rowStart = 0;
	fixRange.rowEnd = state->patterns.rows()-1;
	fixRange.colStart = state->patterns.columns();
	state->patterns.resize(state->patterns.rows(), state->patterns.columns() + sampleSize);
	fixRange.colEnd = state->patterns.columns()-1;

	state->patterns.createBuffers();
	state->patterns.fixRange(fixRange);
	state->patterns.matrix = MatrixXd::Constant(state->patterns.rows(),state->patterns.columns(),rank);
	state->expression.conservativeResize(state->coefficients.rows(),state->patterns.columns());

	int smallCol = systemSize;
	for(auto c: cGroups){
		int cSize;
		MPI_Group_size(c,&cSize);
		if(cSize < smallCol){
			smallCol = cSize;
		}
	}
	vector<BlockSet> shareSets;
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
		MPI_Group_incl(worldGroup,nSize,&NGA[0],&shareSets[i].group);
		MPI_Comm_create(MPI_COMM_WORLD,shareSets[i].group,&shareSets[i].comm);
	}
	shareSet = shareSets[rank%smallCol];
}

void BlockThrow::run(){

	//XXX: (Z_[a/2])^2 *p*(1-p)/(M.O.E)^2 = X   -----   N*X/(X+N-1) = *n*

	state->both = true;
	double error = 0;

	algorithm->setObserver(this);
	algorithm->monteCarlo();
	averagePatterns();
	averageCoefficients();
	error = algorithm->anneal();
	averagePatterns();
	averageCoefficients();

	Range s = block;
	s.rowEnd = state->patterns.rows()-1;
	s.colEnd -= s.colStart;
	s.rowStart = 0;
	s.colStart = 0;
	state->patterns.shrink(s);

	gatherPatterns();
	gatherCoefficients();
}

void BlockThrow::throwPatterns(){
	cout << "Sample Size: " << sampleSize << endl;
	cout << "Throwing" << endl;
	int count = 0;
	Range colGrab;
	int cSize;
	MPI_Comm_size(shareSet.comm, &cSize);
	//Set receive buffer size
	int intake = sampleSize*(state->patterns.rows()+1)*cSize;
	double recvBuf[intake];
	cout << "Set Buffers" << endl;

	//Generate random column number and check if it hasn't been used before
	set<int> pushed;
	bool unique = true;
	for(int i = 0; i < sampleSize; i++){
		int rCol = rand()%state->patterns.columns();
		while(!unique){
			unique = true;
			if(pushed.find(rCol) != pushed.end()){
				rCol = rand()%state->patterns.columns();
				unique = false;
			}
		}
		if(rank == 0)	cout << rCol << endl;
		pushed.insert(rCol);

		//Set column number and data for that column into buffer
		state->patterns.sendBuffer[i*(state->patterns.rows()+1)] = rCol;
		colGrab.rowStart = 0;
		colGrab.colStart = rCol;
		colGrab.rowEnd = state->patterns.rows()-1;
		colGrab.colEnd = rCol;
		state->patterns.write(&state->patterns.sendBuffer[i*(state->patterns.rows()+1)+1],colGrab);
		cout << "Wrote Buffer " << i << endl;
	}

	cout << "Gathering..." << endl;
	MPI_Allgather(state->patterns.sendBuffer,sampleSize*(state->patterns.rows()+1),MPI_DOUBLE,recvBuf,intake,MPI_DOUBLE,shareSet.comm);
	cout << "Gathered" << endl;
	double* ptr = recvBuf;
	//Create set of already seen columns, starting with own columns
	set<int> added;
	for(int i = block.rowStart; i <= block.rowEnd; i++){
		added.insert(i);
	}
	colGrab.colStart = state->patterns.columns()-sampleSize;
	colGrab.colEnd = state->patterns.columns()-sampleSize;
	while(count < sampleSize){
		if(added.find(*ptr) != added.end()){
			state->patterns.read((ptr+1),colGrab);
			state->patterns.observeRange(colGrab);
			added.insert(*ptr);
			colGrab.colStart++;
			colGrab.colEnd++;
			count++;
		}
		ptr += state->patterns.rows()+1;
	}
	delete[] ptr;
}

void BlockThrow::monteCallback(int iter){
	if(state->both && iter/state->interruptRuns%2 == 0){
		averagePatterns();
		throwPatterns();
	}else{
		averageCoefficients();
	}
}

bool BlockThrow::annealCallback(int iter){
	if(state->both && iter/state->interruptRuns%2 == 0){
		averagePatterns();
		throwPatterns();
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
