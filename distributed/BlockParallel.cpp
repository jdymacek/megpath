//Block Parallel file
//Julian Dymacek
//Justin Moore
//Created: 5/30/2019
//Last Modified: 5/30/2019

#include "BlockParallel.h"
#include "../shared/State.h"
#include <vector>
#include <set>
#include <iostream>

BlockParallel::BlockParallel(): ParallelPatterns(){
	program = "BlockParallel";
}

void BlockParallel::start(){
	Distributed::start();
	vector<Range> ar;

	for(int i = 0; i < size; i++){
		ar.push_back(state->getRange(i));
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
		state->patterns.resize(state->patterns.rows, block.colSize());
		//split the coefficients
		state->coefficients.resize(block.rowSize(), state->coefficients.columns);

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
			disp = block.rowStart*state->coefficients.columns;
		}
	}

	pCount = 0;
	pDisp = 0;
	for(int r : rowSets[0]){
		if(rank == r){
			pCount = state->patterns.matrix.size();
			pDisp = block.colStart*state->patterns.rows;
		}
	}

	MPI_Group worldGroup;
	MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);

	MPI_Group group;
	MPI_Comm comm;

	for(auto s: rowGroups){
		vector<int> v(s.begin(),s.end());
		MPI_Group_incl(worldGroup,v.size(),&v[0], &group);
		MPI_Comm_create(MPI_COMM_WORLD,group,&comm);
		if(comm != MPI_COMM_NULL){
			rComms.push_back(comm);
			rGrps.push_back(group);
		}
	}

	for(auto s: colGroups){
		vector<int> v(s.begin(),s.end());
		MPI_Group_incl(worldGroup,v.size(),&v[0], &group);
		MPI_Comm_create(MPI_COMM_WORLD,group,&comm);
		if(comm != MPI_COMM_NULL){
			cComms.push_back(comm);
			cGrps.push_back(group);
		}
	}
//	MPI_Group_free(&worldGroup);

}

void BlockParallel::run(){	
	state->coefficients.matrix = MatrixXd::Constant(state->coefficients.rows,state->coefficients.columns,rank);
	state->patterns.matrix = MatrixXd::Constant(state->patterns.rows,state->patterns.columns,rank);


	for(auto r : rComms){
		allAverage(state->coefficients,r);
	}
	for(auto c : cComms){
		allAverage(state->patterns,c);
	}

	gatherCoefficients();
	if(rank == 0){
		cout << state->coefficients.matrix << endl;
	}

	gatherPatterns();
	if(rank == 0){
		cout << state->patterns.matrix << endl;
	}
	
}

void BlockParallel::gatherPatterns(){
	double* buffer = NULL;
	int  allCounts[size];
	int  allDispls[size];
	double* sendBuf = new double[state->patterns.matrix.size()];

	if(rank == 0){
		buffer = new double[oexpression.cols()*state->patterns.matrix.rows()];
	}

	MPI_Gather(&pCount,1,MPI_INT,&allCounts[0],1,MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Gather(&pDisp,1,MPI_INT,&allDispls[0],1,MPI_INT, 0, MPI_COMM_WORLD);

	MatrixXd ct = state->patterns.matrix.transpose();

	copy(ct.data(),ct.data()+ct.size(), sendBuf);

	MPI_Gatherv(sendBuf, pCount, MPI_DOUBLE, buffer, allCounts, allDispls, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	state->patterns.resize(state->patterns.matrix.rows(),oexpression.cols());
	state->expression = oexpression;
	if(rank == 0){
		double* nb = buffer;
		MatrixXd temp = MatrixXd::Zero(state->patterns.matrix.rows(),oexpression.cols());
		for(int i =0; i < temp.rows(); ++i){
			for(int j=0; j < temp.cols(); ++j){
				temp(i,j) = *buffer;
				buffer += 1;
			}
		}
		state->patterns.matrix = temp;
	//	ErrorFunctionRow efRow(state);
	//	double error = efRow.error();

	//	cout << "Final Error: " << error << endl;
	//	cout << "Patterns: " << endl;
	//	cout << state->patterns.matrix << endl;;
		delete[] nb;
	}
	delete[] sendBuf;
}

void BlockParallel::stop(){
	for(auto r : rComms){
		MPI_Comm_free(&r);
	}
	for(auto c : cComms){
		MPI_Comm_free(&c);
	}
	for(auto r : rGrps){
		MPI_Group_free(&r);
	}
	for(auto c : cGrps){
		MPI_Group_free(&c);
	}
}
