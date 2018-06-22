#include "Distributed.h"

Distributed::Distributed():Analysis(){
	program = "Distributed";
}
Distributed::~Distributed(){
	MPI_Finalize();
}

void Distributed::timedRun(int runTime){
	string toSet = state->stats;
	state->stats = "none";
	double prevTime;
	int prevRuns;
	Stopwatch watch;
	int running = 1;
	double deltaT = 0;
	int deltI = 0;
	while(running > 0 ){
		watch.start();
		start();
		run();
		stop();
		double time = watch.stop();
		if(time > runTime-0.5 && time < runTime+0.5){
			running = -5;
		}else if (running == 1){
			//running = 2;
			prevRuns = state->MAX_RUNS;
			prevTime = time;
			state->MAX_RUNS = broadcastInt(state->MAX_RUNS * runTime/time);
		}else{
			double deltaT = ((prevTime - time));
			int deltaI = ((prevRuns - state->MAX_RUNS));
			//if(deltaT > 1){	
				prevTime = time;
				prevRuns = state->MAX_RUNS;
				int nextRuns = (deltaI/deltaT) * runTime;
				state->MAX_RUNS = broadcastInt(nextRuns);
			//}
		}
		if(state->MAX_RUNS > prevRuns-1 && state->MAX_RUNS < prevRuns+1){
			running = -5;
		}
		running ++;
		//cout << "time/runs: " << time << ":" << state->MAX_RUNS << endl;
	}
	state->stats = toSet;
}
void Distributed::start(){
	Analysis::start();
	int flag = 0;
    	MPI_Initialized(&flag);
	if(flag == 0){
		MPI_Init(NULL,NULL);
    	}
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(flag == 0){
    		program += "_" + to_string(size) + "n";	
	}
    	char hostbuff[100];
    	gethostname(hostbuff,99);
    	hostname = string(hostbuff);
}

void Distributed::sendLeastError(int process, double formerError)
{
	int tag = 0;
	MPI_Status status;

	double* recvBuffer = new double[size];

	MPI_Allgather(&formerError,1, MPI_DOUBLE, recvBuffer, 1, MPI_DOUBLE, MPI_COMM_WORLD);

	//find the smallest
	int smallest = -1;
	double minError = state->coefficients.matrix.size()*2;
	for(int i =0; i < size; ++i){
		if(minError > recvBuffer[i]){
			minError = recvBuffer[i];
			smallest = i;
		}
	}

	//if i have the smallest send
	if(smallest == rank && rank == process){
		cout << hostname << " has the smallest error: " << formerError << endl;
	}else if(smallest == rank){
		cout << hostname << " has the smallest error: " << formerError << endl;
		sendMatrix(state->patterns.matrix, process);
		sendMatrix(state->coefficients.matrix, process);
	}else if(rank == process){
		//recieve info
		recvMatrix(state->patterns.matrix,smallest);	
		recvMatrix(state->coefficients.matrix,smallest);
	}
	delete[] recvBuffer;
}

void Distributed::montePrintCallback(int iter){
	cout << hostname << "\t";
	Analysis::montePrintCallback(iter);
}

void Distributed::annealPrintCallback(int iter){
	cout << hostname << "\t";
	Analysis::annealPrintCallback(iter);
}


void Distributed::sendMatrix(MatrixXd& matrix,int dest){
	int tag = 0;
	MPI_Status status;
	MPI_Send(matrix.data(),matrix.rows()*matrix.cols(),MPI_DOUBLE,dest,tag,MPI_COMM_WORLD);
}

void Distributed::recvMatrix(MatrixXd& matrix,int src){
	int tag = 0;
	MPI_Status status;
	MPI_Recv(matrix.data(),matrix.rows()*matrix.cols(),MPI_DOUBLE,src,tag,MPI_COMM_WORLD,&status);
}

int Distributed::broadcastInt(int toSend){
	int tag = 0;
	MPI_Status status;
	int rv = toSend;
	MPI_Bcast(&rv,1,MPI_INT,0,MPI_COMM_WORLD);
	return rv;
}
void Distributed::run(){

}

void Distributed::stop(){
	Analysis::stop();
	if(rank == 0){
		Analysis::output();
		cout << "Total program running time: " << ttime << endl;
	}
}
