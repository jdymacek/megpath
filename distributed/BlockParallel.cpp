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
		set<vector<int>>::iterator it;
		for(int i = 0; i < state->expression.rows(); i++){
			vector<int> row;
			for(int j = 0; j < ar.size(); j++){
				if(i >= ar[j].rowStart && i <= ar[j].rowEnd){
					row.push_back(j);
				}
			}
//			for(int j = 0; j < row.size(); j++){
//				cout << row[j] << '\t';
//			}
//			cout << endl;
			if(row.size() > 0)
				rowGroups.insert(row);
		}

		cout << "Row Amount: " << rowGroups.size() << endl;
		for(it = rowGroups.begin(); it != rowGroups.end(); it++){
			const vector<int> rowOut = (*it);
			for(int j = 0; j<rowOut.size(); j++){
				cout << rowOut[j] << '\t';
			}
			cout << endl;
		} 

		set<vector<int>> colGroups;
		for(int i = 0; i < state->expression.cols(); i++){
			vector<int> col;
			for(int j = 0; j < ar.size(); j++){
				if(i >= ar[j].colStart && i <= ar[j].colEnd){
					col.push_back(j);
				}
			}
//			for(int j = 0; j < col.size(); j++){
//				cout << col[j] << '\t';
//			}
//			cout << endl;
			if(col.size() > 0)
				colGroups.insert(col);
		}

		cout << "Col Amount: " << colGroups.size() << endl;
		for(it = colGroups.begin(); it != colGroups.end(); it++){
			const vector<int> colOut = (*it);
			for(int j = 0; j<colOut.size(); j++){
				cout << colOut[j] << '\t';
			}
			cout << endl;
		} 
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
