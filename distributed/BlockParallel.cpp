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
		Range r = state->getRange(i);
		ar.push_back(r);
	}

	set<vector<int>> rowGroups;
	for(int i = 0; i < state->expression.rows(); i++){
		vector<int> row;
		for(int j = 0; j < ar.size(); j++){
			if(i >= ar[j].rowStart && i <= ar[j].rowEnd){
				row.push_back(j);
			}
		}
		if(row.size() > 0)
			rowGroups.insert(row);
	}

	set<vector<int>> colGroups;
	for(int i = 0; i < state->expression.cols(); i++){
		vector<int> col;
		for(int j = 0; j < ar.size(); j++){
			if(i >= ar[j].colStart && i <= ar[j].colEnd){
				col.push_back(j);
			}
		}
		if(col.size() > 0)
			colGroups.insert(col);
	}

	MPI_Group world_group;
	MPI_Comm_group(MPI_COMM_WORLD, &world_group);

	vector<MPI_Group> grps;
	vector<MPI_Comm> comms;
	for(vector<int> row : rowGroups){
		bool in = false;
		for(int i = 0; i < row.size(); i++){
			if(rank == row[i]){
				in = true;
				break;
			}
		}
		if(in){
			MPI_Group groupR;
			MPI_Comm commR;
			int n = row.size();
			int vtoa[n];
			for(int i = 0; i < n; i++){
				vtoa[i] = row[i];
			}
			MPI_Group_incl(world_group, n, vtoa, &groupR);
			grps.push_back(groupR);
			MPI_Comm_create(MPI_COMM_WORLD, groupR, &commR);
			comms.push_back(commR);
		}
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
