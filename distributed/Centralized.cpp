//Centralized file
//Julian Dymacek
//Justin Moore
//Created: 6/18/2019
//Last Modified: 6/18/2019

#include "Centralized.h"
#include <time.h>

Centralized::Centralized(): BlockParallel(){
	program = "Centralized";
}

void Centralized::start(){
	Distributed::start();
	srand(time(0));
	oexpression = state->expression;
	if(rank == 0){

	}else{
		vector<int> parse;

		int push = 0;
		for(int i = 0; i<state->dist.size(); i++){
			if(state->dist[i] >= '0' && state->dist[i] <= '9'){
				push = push*10 + state->dist[i]-'0';
			}

			if(state->dist[i] == '+' || i == state->dist.size()-1){
				if(push == 0)
					push = 1;
				parse.push_back(push);
				push = 0;
				if(parse.size()%2 == 1)
					parse.push_back(1);
			}else if(state->dist[i] == '*' || state->dist[i] == 'x'){
				if(push == 0)
					push = 1;
				parse.push_back(push);
				push = 0;
			}
		}
		int distSize = 0;
		for(int i = 0; i< parse.size(); i+=2){
			distSize += (parse[i]*parse[i+1]);
		}
		block = state->getRange((rank-1)%distSize);

		if(block.isValid()){
			state->patterns.resize(state->patterns.rows(), block.colSize());
			state->coefficients.resize(block.rowSize(), state->coefficients.columns());

			MatrixXd temp = state->expression.block(block.rowStart, block.colStart, block.rowSize(), block.colSize());
			state->expression = temp;

		}
	}
	state->patterns.createBuffers();
	state->coefficients.createBuffers();
}

void Centralized::run(){
	state->both = true;
	double error = 0;

	if(rank == 0){
		int workingSize = systemSize-1;
		int runs = 0;
		int indexOne = 1000;
		while(workingSize > 0){
			MPI_Status status;
			Range received;
			MPI_Recv(&received,4,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
			if(received.rowStart == -1){
				workingSize--;
			}else{
				Range coRec = received;
				coRec.colStart = 0;
				coRec.colEnd = state->coefficients.columns()-1;
				Range patRec = received;
				patRec.rowStart = 0;
				patRec.rowEnd = state->patterns.rows()-1;
				if(status.MPI_SOURCE == 1){
					string out = "centralpat/debug" + to_string(indexOne) + ".png";
					state->unshufflePC();
					state->MXdToPNG(state->patterns.matrix,state->patterns.rows(),state->patterns.columns(),false,out.c_str());
//					state->MXdToPNG(state->coefficients.matrix,state->coefficients.rows(),state->coefficients.columns(),true,out.c_str());
					state->reshufflePC();
					++indexOne;
				}
				double a = rand()%51/1000.0 + 0.05;
				MPI_Recv(&state->coefficients.sendBuffer[0],state->coefficients.size(coRec),MPI_DOUBLE,status.MPI_SOURCE,1,MPI_COMM_WORLD,&status);
				state->coefficients.average(&state->coefficients.sendBuffer[0],coRec,a);

				MPI_Recv(&state->patterns.sendBuffer[0],state->patterns.size(patRec),MPI_DOUBLE,status.MPI_SOURCE,2,MPI_COMM_WORLD,&status);
				state->patterns.average(&state->patterns.sendBuffer[0],patRec,a);
				if(status.MPI_SOURCE == 1){
					string out = "centralpat/debug" + to_string(indexOne) + ".png";
					state->unshufflePC();
					state->MXdToPNG(state->patterns.matrix,state->patterns.rows(),state->patterns.columns(),false,out.c_str());
//					state->MXdToPNG(state->coefficients.matrix,state->coefficients.rows(),state->coefficients.columns(),true,out.c_str());
					state->reshufflePC();
					++indexOne;
				}

				int rSiz = received.rowSize();
				int cSiz = received.colSize();
				int xRange = oexpression.cols()+cSiz;
				int yRange = oexpression.rows()+rSiz;
				int randX = rand()%xRange - cSiz;
				if(randX < 0){
					randX = 0;
				}else if(randX > oexpression.cols()-cSiz){
					randX = oexpression.cols()-cSiz;
				}
				int randY = rand()%yRange - rSiz;
				if(randY < 0){
					randY = 0;
				}else if(randY > oexpression.rows()-rSiz){
					randY = oexpression.rows()-rSiz;
				}
				received.rowStart = randY;
				received.rowEnd = randY + rSiz - 1;
				received.colStart = randX;
				received.colEnd = randX + cSiz - 1;
				MPI_Send(&received,4,MPI_INT,status.MPI_SOURCE,0,MPI_COMM_WORLD);

				coRec = received;
				coRec.colStart = 0;
				coRec.colEnd = state->coefficients.columns()-1;
				state->coefficients.write(&state->coefficients.sendBuffer[0],coRec);
				MPI_Send(&state->coefficients.sendBuffer[0],state->coefficients.size(coRec),MPI_DOUBLE,status.MPI_SOURCE,1,MPI_COMM_WORLD);

				patRec = received;
				patRec.rowStart = 0;
				patRec.rowEnd = state->patterns.rows()-1;
				state->patterns.write(&state->patterns.sendBuffer[0],patRec);
				MPI_Send(&state->patterns.sendBuffer[0],state->patterns.size(patRec),MPI_DOUBLE,status.MPI_SOURCE,2,MPI_COMM_WORLD);
			}
			runs += 1;
			if(runs % 1000 == 0){
				ErrorFunctionRow efRow(state);
				error = efRow.error();
				cout << "Current Error: " << error/state->expression.size() << endl;
			}
		}
		ErrorFunctionRow efRow(state);
		error = efRow.error();

		cout << "Final Error: " << error/state->expression.size() << endl;
		state->errorToPNG();
	}else{
		algorithm->setObserver(this);
		algorithm->monteCarlo();
		monteCallback(0);
		error = algorithm->anneal();
		annealCallback(0);
		block = {-1,-1,-1,-1};
		MPI_Send(&block,4,MPI_INT,0,0,MPI_COMM_WORLD);
	}
}


void Centralized::monteCallback(int iter){


	MPI_Status status;
	MPI_Send(&block,4,MPI_INT,0,0,MPI_COMM_WORLD);

	state->coefficients.write(&state->coefficients.sendBuffer[0]);
	MPI_Send(&state->coefficients.sendBuffer[0],state->coefficients.size(),MPI_DOUBLE,0,1,MPI_COMM_WORLD);

	state->patterns.write(&state->patterns.sendBuffer[0]);
	MPI_Send(&state->patterns.sendBuffer[0],state->patterns.size(),MPI_DOUBLE,0,2,MPI_COMM_WORLD);

	MPI_Recv(&block,4,MPI_INT,0,0,MPI_COMM_WORLD,&status);

	MPI_Recv(&state->coefficients.recvBuffer[0],state->coefficients.size(),MPI_DOUBLE,status.MPI_SOURCE,1,MPI_COMM_WORLD,&status);
	state->coefficients.read(&state->coefficients.recvBuffer[0]);

	MPI_Recv(&state->patterns.recvBuffer[0],state->patterns.size(),MPI_DOUBLE,status.MPI_SOURCE,2,MPI_COMM_WORLD,&status);
	state->patterns.read(&state->patterns.recvBuffer[0]);


	MatrixXd temp = oexpression.block(block.rowStart, block.colStart, block.rowSize(), block.colSize());
	state->expression = temp;

}

bool Centralized::annealCallback(int iter){
	MPI_Status status;
	MPI_Send(&block,4,MPI_INT,0,0,MPI_COMM_WORLD);

	state->coefficients.write(&state->coefficients.sendBuffer[0]);
	MPI_Send(&state->coefficients.sendBuffer[0],state->coefficients.size(),MPI_DOUBLE,0,1,MPI_COMM_WORLD);

	state->patterns.write(&state->patterns.sendBuffer[0]);
	MPI_Send(&state->patterns.sendBuffer[0],state->patterns.size(),MPI_DOUBLE,0,2,MPI_COMM_WORLD);

	MPI_Recv(&block,4,MPI_INT,0,0,MPI_COMM_WORLD,&status);

	MPI_Recv(&state->coefficients.recvBuffer[0],state->coefficients.size(),MPI_DOUBLE,status.MPI_SOURCE,1,MPI_COMM_WORLD,&status);
	state->coefficients.read(&state->coefficients.recvBuffer[0]);

	MPI_Recv(&state->patterns.recvBuffer[0],state->patterns.size(),MPI_DOUBLE,status.MPI_SOURCE,2,MPI_COMM_WORLD,&status);
	state->patterns.read(&state->patterns.recvBuffer[0]);

	MatrixXd temp = oexpression.block(block.rowStart, block.colStart, block.rowSize(), block.colSize());
	state->expression = temp;
	return true;
}
