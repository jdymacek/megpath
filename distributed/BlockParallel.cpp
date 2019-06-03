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

	block = ar[rank];

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
			rGrps.push_back(groupR);
			MPI_Comm_create(MPI_COMM_WORLD, groupR, &commR);
			rComms.push_back(commR);
		}
	}
	for(vector<int> col : colGroups){
		bool in = false;
		for(int i = 0; i < col.size(); i++){
			if(rank == col[i]){
				in = true;
				break;
			}
		}
		if(in){
			MPI_Group groupC;
			MPI_Comm commC;
			int n = col.size();
			int vtoa[n];
			for(int i = 0; i < n; i++){
				vtoa[i] = col[i];
			}
			MPI_Group_incl(world_group, n, vtoa, &groupC);
			cGrps.push_back(groupC);
			MPI_Comm_create(MPI_COMM_WORLD, groupC, &commC);
			cComms.push_back(commC);
		}
	}
/*
	//if(rank == 0){
		oexpression = state->expression;
	//}

	//split the coefficients
	int rowSect = block.rowEnd - block.rowStart;
	state->coefficients.resize(rowSect, state->coefficients.columns);
	//split the patterns
	int colSect = block.colEnd - block.colStart;
	state->patterns.resize(colSect, state->patterns.rows);

	//change to Range rowStart
	//startPoint = r.rowStart;

	//replace with the 4 Range coordinates
	MatrixXd temp = state->expression.block(block.rowStart, block.colStart, rowSect, colSect);
	state->expression = temp;

	bufferSize = state->patterns.size();
	sendBuffer = new double[bufferSize];
	recvBuffer = new double[bufferSize];
*/
}

void BlockParallel::run(){
	double* buffer = NULL;
	int send = state->coefficients.matrix.size();
	int gRank;
	int gSize;
	for(int i = 0; i < rComms.size(); i++){
		MPI_Comm_rank(rComms[i], &gRank);
		MPI_Comm_size(rComms[i], &gSize);
//		int gCounts[gSize];
//		int gDispls[gSize];
		int res;
		MPI_Allreduce(&rank,&res,1,MPI_INT,MPI_MAX,rComms[i]);
		cout << rank << '\t' << rComms[i] << '\t' << "Max: " << res << endl;
		MPI_Allreduce(&rank,&res,1,MPI_INT,MPI_MIN,rComms[i]);
		cout << rank << '\t' << rComms[i] << '\t' << "Min: " << res << endl;
		MPI_Allreduce(&rank,&res,1,MPI_INT,MPI_PROD,rComms[i]);
		cout << rank << '\t' << rComms[i] << '\t' << "Prd: " << res << endl;
/*		MPI_Allgather(&rank,1,MPI_INT,&gCounts[0],1,MPI_INT,rComms[i]);
		MPI_Allgather(&send,1,MPI_INT,&gDispls[0],1,MPI_INT,rComms[i]);
		for(int r : gCounts){
			cout << rank << '\t' << rComms[i] << '\t' << r << endl;
		}
*/
	}
	for(int i = 0; i < cComms.size(); i++){
		MPI_Comm_rank(cComms[i], &gRank);
		MPI_Comm_size(cComms[i], &gSize);
//		int gCounts[gSize];
//		int gDispls[gSize];
		int res;
		MPI_Allreduce(&rank,&res,1,MPI_INT,MPI_MAX,cComms[i]);
		cout << rank << '\t' << cComms[i] << '\t' << "Max: " << res << endl;
		MPI_Allreduce(&rank,&res,1,MPI_INT,MPI_MIN,cComms[i]);
		cout << rank << '\t' << cComms[i] << '\t' << "Min: " << res << endl;
		MPI_Allreduce(&rank,&res,1,MPI_INT,MPI_PROD,cComms[i]);
		cout << rank << '\t' << cComms[i] << '\t' << "Prd: " << res << endl;
/*		MPI_Allgather(&rank,1,MPI_INT,&gCounts[0],1,MPI_INT,cComms[i]);
		MPI_Allgather(&send,1,MPI_INT,&gDispls[0],1,MPI_INT,rComms[i]);
		for(int c : gCounts){
			cout << rank << '\t' << cComms[i] << '\t' << c << endl;
		}
*/
	}
	
//	state->patterns.write(&sendBuffer[0]);
//	MPI_Allreduce(sendBuffer, recvBuffer, bufferSize, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
}
