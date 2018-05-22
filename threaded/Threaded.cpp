//Threaded functions
//Dakota Martin
//Julian Dymacek
//Created on 5/22/2018

#include "Threaded.h"

Threaded::Threaded(int nt){
	numThreads = nt;
	ProbFunc::generator.seed(time(0));
	uniform = new UniformPF();
	program = "Threaded";
	barrier = new Barrier(numThreads);
}

/*Run a monte carlo markov chain*/
void Threaded::monteCarloThread(int xStart, int xEnd,int yStart,int yEnd){
	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);

	//For each spot take a gamble and record outcome
	for(int i =0; i < state->MAX_RUNS; i++){
		if(state->both){
			monteCarloStep(state->patterns,&efCol,xStart,xEnd,yStart,yEnd);
			barrier->wait();
		}
		monteCarloStep(state->coefficients,&efRow,xStart,xEnd,yStart,yEnd);
		if(state->both){
			barrier->wait();
		}
	}
}


double Threaded::monteCarlo(){
    Stopwatch watch;
    watch.start();
	vector<thread> threads;

	int rowSize = state->rows/numThreads;
	int colSize = state->cols/numThreads;
	int rowStart = 0;
	int colStart = 0;
	for(int i =0; i < numThreads; ++i){
		int rowEnd = rowStart + rowSize;
		int colEnd = colStart + colSize;
		if(i < state->rows%numThreads)
			rowEnd += 1;
		if(i < state->cols%numThreads)
			colEnd += 1;
		threads.push_back(thread(Threaded::monteCarloThead,this,colStart,colEnd,rowStart,rowEnd));
		rowStart = rowEnd;
		colStart = colEnd;
	}

	for(int i =0; i < threads.size();++i){
		threads[i].join();
	}

	ErrorFunctionRow efRow(state);
    cout << "Final Error: " << efRow.error() << endl;
    cout << "Error Histogram: " << efRow.errorDistribution(10) << endl;
    cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
    return efRow.error();
}


double Threaded::anneal(){
	Stopwatch watch;
	int ndx = 0;
	double t = 0.5;
	watch.start();

	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);


	double formerError = 2*state->expression.rows()*state->expression.cols();
	bool running = true;
	while(running && ndx < 2*state->MAX_RUNS){
		annealStep(state->coefficients,t,&efRow);
		if(state->both){
			annealStep(state->patterns,t,&efCol);
		}
		if(ndx % state->printRuns == 0){
			double error = efRow.error();
			cout << ndx << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
			if(abs(formerError - error) < 0.005 && error < formerError)
				running = false;
			formerError = error;
		}
		ndx++;
		t *= 0.99975;
	}
	cout << "Final Error: " << efRow.error() << endl;
	cout << "Error Histogram: " << efRow.errorDistribution(10) << endl;
	cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
	return efRow.error();
}

void Threaded::run(){
	//Could put stop watch in here
	ProbFunc::generator.seed(time(0));
	monteCarlo();
//	anneal();		
}

void Threaded::stop(){
	//state->patterns.write(state->analysis + "patterns.csv");
	//state->coefficients.write(state->analysis + "coefficients.csv");

	//ofstream fout;
	//fout.open(state->analysis + "expression.txt");
	//fout << state->coefficients.matrix*state->patterns.matrix;
	//fout.close();
}

