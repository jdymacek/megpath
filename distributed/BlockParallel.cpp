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

	if(rank == 0){
		set<vector<int>> rowGroups;
		bool same = false;
		for(int i = 0; i < state->expression.rows(); i++){
			vector<int> row;
			for(int j = 0; j < ar.size(); j++){
				if(i >= ar[j].rowStart && i <= ar[j].rowEnd){
					row.push_back(j);
				}
			}
			for(int j = 0; j < row.size(); j++){
				cout << row[j] << '\t';
			}
			cout << endl;
			rowGroups.insert(row);
		}

		cout << "Set Amount: " << rowGroups.size() << endl;
	}
	/*	//if(rank == 0){
		oexpression = state->expression;
	//}

	//vector<vector<int>> ranges = state->splitRanges(size);
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
