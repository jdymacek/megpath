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
		block = state->getRange(rank-1);

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

				MPI_Recv(&state->coefficients.sendBuffer[0],state->coefficients.size(coRec),MPI_DOUBLE,status.MPI_SOURCE,1,MPI_COMM_WORLD,&status);
				state->coefficients.average(&state->coefficients.sendBuffer[0],coRec);

				MPI_Recv(&state->patterns.sendBuffer[0],state->patterns.size(patRec),MPI_DOUBLE,status.MPI_SOURCE,2,MPI_COMM_WORLD,&status);
				state->patterns.average(&state->patterns.sendBuffer[0],patRec);

				int randX = rand()%(oexpression.cols()-received.colSize()+1);
				int randY = rand()%(oexpression.rows()-received.rowSize()+1);
				int rSiz = received.rowSize();
				int cSiz = received.colSize();
				received.rowStart = randY;
				received.rowEnd = received.rowStart + rSiz - 1;
				received.colStart = randX;
				received.colEnd = received.colStart + cSiz - 1;
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
		}
		ErrorFunctionRow efRow(state);
		error = efRow.error();

		cout << "Final Error: " << error/state->expression.size() << endl;
	}else{
		algorithm->setObserver(this);
		algorithm->monteCarlo();
		monteCallback(0);
		error = algorithm->anneal();
		annealCallback(0);
		block.rowStart = -1;
		block.rowEnd = -1;
		block.colStart = -1;
		block.colEnd = -1;
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
