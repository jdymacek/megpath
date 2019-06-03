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

BlockParallel::BlockParallel(): Distributed(){
	program = "BlockParallel";
}

void BlockParallel::start(){
	Distributed::start();
	vector<Range> ar;

	for(int i = 0; i < size; i++){
		ar.push_back(state->getRange(i));
	}


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

	MPI_Group worldGroup;
	MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);

	//seperate groups for rows and columns?  
	vector<MPI_Group> grps;
	vector<MPI_Comm> comms;
	MPI_Group groupR;
	MPI_Comm commR;

	for(auto s: rowGroups){
		vector<int> v(s.begin(),s.end());
		MPI_Group_incl(worldGroup,v.size(),&v[0], &groupR);
		MPI_Comm_create(MPI_COMM_WORLD,groupR,&commR);
		comms.push_back(commR);
		grps.push_back(groupR);
	}



	/*	//if(rank == 0){
		oexpression = state->expression;
	//}

	Range r = state->getRange(rank);
	//split the coefficients
	int rowSect = r.rowEnd - r.rowStart;
	state->coefficients.resize(rowSect, state->coefficients.columns);
	//split the patterns
	int colSect = r.colEnd - r.colStart;
	state->patterns.resize(colSect, state->patterns.rows);

	//change to Range rowStart
	//startPoint = r.rowStart;

	//replace with the 4 Range coordinates
	MatrixXd temp = state->expression.block(r.rowStart, r.colStart, rowSect, colSect);
	state->expression = temp;

	bufferSize = state->patterns.size();
	sendBuffer = new double[bufferSize];
	recvBuffer = new double[bufferSize];
	*/
}
