//Block Parallel file
//Julian Dymacek
//Justin Moore
//Created: 5/30/2019
//Last Modified: 5/30/2019

#include "BlockParallel.h"
#include <set>
#include <cmath>

BlockParallel::BlockParallel(): ParallelPatterns(){
	program = "BlockParallel";
}

void BlockParallel::start(){
	Distributed::start();
	vector<Range> ar;

	for(int i = 0; i < systemSize; i++){
		Range r = state->getRange(i);
		ar.push_back(r);
	}

	block = ar[rank];

	vector<set<int>> rowSets(state->expression.rows());
	vector<set<int>> colSets(state->expression.cols());

	for(int i =0; i < state->expression.rows(); ++i){
		for(int j = 0; j < state->expression.cols(); ++j){
			for(int r = 0; r < ar.size(); ++r){
				if(ar[r].contains(i,j)){
					rowSets[i].insert(r);
					colSets[j].insert(r);
				}	
			}
		}
	}

	set<set<int>> rowGroups(rowSets.begin(),rowSets.end());
	set<set<int>> colGroups(colSets.begin(),colSets.end());
	
	if(block.isValid()){
		//if(rank == 0){
		oexpression = state->expression;
		//}

		//split the patterns
		state->patterns.resize(state->patterns.rows(), block.colSize());
		//split the coefficients
		state->coefficients.resize(block.rowSize(), state->coefficients.columns());

		MatrixXd temp = state->expression.block(block.rowStart, block.colStart, block.rowSize(), block.colSize());
		state->expression = temp;
		
		state->patterns.createBuffers();
		state->coefficients.createBuffers();
	}

	rowUnique = colSets[0].size();
	count = 0;
	disp = 0;
	for(int r : colSets[0]){
		if(rank == r){
			count = state->coefficients.matrix.size();
			disp = block.rowStart*state->coefficients.columns();
		}
	}

	pCount = 0;
	pDisp = 0;
	for(int r : rowSets[0]){
		if(rank == r){
			pCount = state->patterns.matrix.size();
			pDisp = block.colStart*state->patterns.rows();
		}
	}

	MPI_Group worldGroup;
	MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);

	BlockSet form;
	form.subRange = block;

	for(auto s: rowGroups){
		vector<int> v(s.begin(),s.end());
		for(int i = 0; i < v.size(); i++){
			if(ar[v[i]].rowStart > form.subRange.rowStart){
				form.subRange.rowStart = ar[v[i]].rowStart;
			}
			if(ar[v[i]].rowEnd < form.subRange.rowEnd){
				form.subRange.rowEnd = ar[v[i]].rowEnd;
			}
		}
		MPI_Group_incl(worldGroup,v.size(),&v[0], &form.group);
		MPI_Comm_create(MPI_COMM_WORLD,form.group,&form.comm);
		if(form.comm != MPI_COMM_NULL){
			rSets.push_back(form);
		}
		form.subRange = block;
	}

	for(auto s: colGroups){
		vector<int> v(s.begin(),s.end());
		int gColSize = oexpression.cols()/v.size();
		sampleSize = sqrt(gColSize);
		for(int i = 0; i < v.size(); i++){
			if(ar[v[i]].colStart > form.subRange.colStart){
				form.subRange.colStart = ar[v[i]].colStart;
			}
			if(ar[v[i]].colEnd < form.subRange.colEnd){
				form.subRange.colEnd = ar[v[i]].colEnd;
			}	
		}
		MPI_Group_incl(worldGroup,v.size(),&v[0], &form.group);
		cGroups.push_back(form.group);
		MPI_Comm_create(MPI_COMM_WORLD,form.group,&form.comm);
		if(form.comm != MPI_COMM_NULL){
			cSets.push_back(form);
		}
		form.subRange = block;
	}
}

void BlockParallel::groupAverage(NMFMatrix& mat, BlockSet set){
	int gSize;
	MPI_Comm_size(set.comm,&gSize);
	if(gSize <= 1)
		return;
	int ret = mat.write(&mat.sendBuffer[0],set.subRange);
	MPI_Allreduce(mat.sendBuffer, mat.recvBuffer, ret, MPI_DOUBLE, MPI_SUM, set.comm);
	for(int q = 0; q < ret; q++){
		mat.recvBuffer[q] /= gSize;
	}
	mat.read(&mat.recvBuffer[0],set.subRange);
}

void BlockParallel::averagePatterns(){
	for(auto c : cSets){
		c.subRange.rowStart = 0;
		c.subRange.colStart -= block.colStart;
		c.subRange.rowEnd = state->patterns.rows()-1;
		c.subRange.colEnd -= block.colStart;
		groupAverage(state->patterns,c);
	}
}

void BlockParallel::averageCoefficients(){
	for(auto r : rSets){
		r.subRange.rowStart -= block.rowStart;
		r.subRange.colStart = 0;
		r.subRange.rowEnd -= block.rowStart;
		r.subRange.colEnd = state->coefficients.columns()-1;
		groupAverage(state->coefficients,r);
	}
}

void BlockParallel::run(){
//	state->patterns.matrix = MatrixXd::Constant(state->patterns.rows,state->patterns.columns,rank);
//	state->coefficients.matrix = MatrixXd::Constant(state->coefficients.rows,state->coefficients.columns,1.0*rank/(systemSize*(systemSize+1)/2));
	state->both = true;
	double error = 0;

	algorithm->setObserver(this);
	algorithm->monteCarlo();
	averagePatterns();
	averageCoefficients();
	error = algorithm->anneal();
	averagePatterns();
	averageCoefficients();

	gatherPatterns();
	gatherCoefficients();
}

void BlockParallel::gatherPatterns(){
	double* buffer = NULL;
	int  allCounts[systemSize];
	int  allDispls[systemSize];
	if(rank == 0){
		buffer = new double[oexpression.cols()*state->patterns.matrix.rows()];
	}

	MPI_Gather(&pCount,1,MPI_INT,&allCounts[0],1,MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Gather(&pDisp,1,MPI_INT,&allDispls[0],1,MPI_INT, 0, MPI_COMM_WORLD);

//	if(rank == 0 || rank == 1){
//		cout << rank << '\n' << state->patterns.matrix << '\n';
//	}

	MPI_Gatherv(state->patterns.matrix.data(), pCount, MPI_DOUBLE, buffer, allCounts, allDispls, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	state->patterns.resize(state->patterns.matrix.rows(),oexpression.cols());
	state->expression = oexpression;
	if(rank == 0){
		Map<MatrixXd> mapper(buffer,state->patterns.matrix.rows(),oexpression.cols());
		state->patterns.matrix = mapper;
//		cout << state->patterns.matrix << endl;
		delete[] buffer;
	}
}

void BlockParallel::monteCallback(int iter){
//	cout << iter << '\t' << rank << '\t' << state->patterns.functions(0,0)->toString() << '\n';
	if(iter/state->interruptRuns%2 == 1){
		averagePatterns();
	}else{
		averageCoefficients();
	}
}

bool BlockParallel::annealCallback(int iter){
	if(state->both && iter >= state->annealCutOff*state->MAX_RUNS){
//		cout << "Before Both\t";
//		annealPrintCallback(iter);
		state->both = false;
//		cout << "After Both\t";
//		annealPrintCallback(iter);
		averagePatterns();
//		cout << "After Avg\t";
//		annealPrintCallback(iter);
	}
	if(state->both && iter/state->interruptRuns%2 == 1){
		averagePatterns();
	}else{
//		if(rank == 1){
//			cout << iter << '\t' << rank << '\t' << state->coefficients.matrix << endl;
//		}
		averageCoefficients();
	}
	return true;
}


void BlockParallel::stop(){
	for(auto r : rSets){
		MPI_Comm_free(&r.comm);
		MPI_Group_free(&r.group);
	}
	for(auto c : cSets){
		MPI_Comm_free(&c.comm);
		MPI_Group_free(&c.group);
	}
	Distributed::stop();
}
