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

	rowUnique = colSets[0].size();
	cout << rowUnique << endl;

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

	if(block.isValid()){
		//if(rank == 0){
		oexpression = state->expression;
		//}

		//split the patterns
		state->patterns.resize(block.colSize(), state->patterns.rows);
		//split the coefficients
		state->coefficients.resize(block.rowSize(), state->coefficients.columns);

		MatrixXd temp = state->expression.block(block.rowStart, block.colStart, block.rowSize(), block.colSize());
		state->expression = temp;

		bufferSize = state->patterns.size();
		sendBuffer = new double[bufferSize];
		recvBuffer = new double[bufferSize];
	}
}

void BlockParallel::run(){
	cout << rank << '\t' << hostname << endl;
	double* buffer = NULL;
//	int send = state->coefficients.matrix.size();
	int gRank;
	int recv[] = {rank, rank};
	int vals[] = {rank, rank};
	int* gBuff = new int[rowUnique*2];
	int* counts = new int[size];
	int* disp = new int[size];
	memset(counts,0,sizeof(int)*size);
	memset(disp,0,sizeof(int)*size);

	int count = 0;
	int dis = 0;

	for(auto r : rComms){
		MPI_Comm_rank(r, &gRank);
		if(gRank == 0){
			count = 2;
			dis = 2*rank;
		}
		MPI_Reduce(vals,&recv[0],2,MPI_INT,MPI_MAX,0,r);
	}

	MPI_Gather(&count,1,MPI_INT,counts,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Gather(&dis,1,MPI_INT,disp,1,MPI_INT,0,MPI_COMM_WORLD);

	if(rank == 0){
		for(int i = 0; i < size; i++){
			cout << counts[i] << ',' << disp[i] << "; ";
		}
		cout << endl;
	}

	MPI_Gatherv(recv,count,MPI_INT,gBuff,counts,disp,MPI_INT,0,MPI_COMM_WORLD);
	cout << rank << " " << hostname << endl;
	
	if(rank == 0){
		for(int i = 0; i < rowUnique*2; i++){
			cout << gBuff[i] << ' ';
		}
		cout << endl;
	}
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
