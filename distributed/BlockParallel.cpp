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

	MPI_Group worldGroup;
	MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);

	MPI_Group group;
	MPI_Comm comm;

	for(auto s: rowGroups){
		vector<int> v(s.begin(),s.end());
		MPI_Group_incl(worldGroup,v.size(),&v[0], &group);
		MPI_Comm_create(MPI_COMM_WORLD,group,&comm);
		rComms.push_back(comm);
		rGrps.push_back(group);
	}

    for(auto s: colGroups){
        vector<int> v(s.begin(),s.end());
        MPI_Group_incl(worldGroup,v.size(),&v[0], &group);
        MPI_Comm_create(MPI_COMM_WORLD,group,&comm);
        cComms.push_back(comm);
        cGrps.push_back(group);
    }

	/*	//if(rank == 0){
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
	cout << rank << '\t' << hostname << endl;
	double* buffer = NULL;
	int send = state->coefficients.matrix.size();
//	int gRank;
//	int gSize;
	for(int i = 0; i < rComms.size(); i++){
//		MPI_Comm_rank(rComms[i], &gRank);
//		MPI_Comm_size(rComms[i], &gSize);
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
//		MPI_Comm_rank(cComms[i], &gRank);
//		MPI_Comm_size(cComms[i], &gSize);
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
