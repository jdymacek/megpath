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
			monteCarloStep(state->patterns,&efCol,xStart,xEnd,0,state->patterns.rows);
			barrier->Wait();
		}
		monteCarloStep(state->coefficients,&efRow,0,state->coefficients.columns,yStart,yEnd);
		if(state->both){
			barrier->Wait();
		}
	}
}


double Threaded::monteCarlo(){
    Stopwatch watch;
    watch.start();
	vector<thread> threads;

	int rowSize = state->coefficients.rows/numThreads;
	int colSize = state->patterns.columns/numThreads;
	int rowStart = 0;
	int colStart = 0;
	for(int i =0; i < numThreads; ++i){
		int rowEnd = rowStart + rowSize;
		int colEnd = colStart + colSize;
		if(i < state->coefficients.rows%numThreads)
			rowEnd += 1;
		if(i < state->patterns.columns%numThreads)
			colEnd += 1;
		cout << "rs:" << rowStart << " re:" << rowEnd << endl << "cs:" << colStart << " ce:" << colEnd << endl << endl;
		threads.push_back(thread(&Threaded::monteCarloThread,this,colStart,colEnd,rowStart,rowEnd));
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


void Threaded::annealThread(int xStart, int xEnd,int yStart,int yEnd){
    ErrorFunctionRow efRow(state);
    ErrorFunctionCol efCol(state);
	double t = 0.5;

    //For each spot take a gamble and record outcome
    for(int i =0; i < 2*state->MAX_RUNS; i++){
        annealStep(state->coefficients,t,&efRow,0,state->coefficients.columns,yStart,yEnd);
        if(state->both){
            barrier->Wait();
			annealStep(state->patterns,t,&efCol,xStart,xEnd,0,state->patterns.rows);
            barrier->Wait(); 
		}
		t *= 0.99975;
    }
}

double Threaded::anneal(){
	Stopwatch watch;
	watch.start();


    vector<thread> threads;

    int rowSize = state->coefficients.rows/numThreads;
    int colSize = state->patterns.columns/numThreads;
    int rowStart = 0;
    int colStart = 0;
    for(int i =0; i < numThreads; ++i){
        int rowEnd = rowStart + rowSize;
        int colEnd = colStart + colSize;
        if(i < state->coefficients.rows%numThreads)
            rowEnd += 1;
        if(i < state->patterns.columns%numThreads)
            colEnd += 1;
        threads.push_back(thread(&Threaded::annealThread,this,colStart,colEnd,rowStart,rowEnd));
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

void Threaded::run(){
	//Could put stop watch in here
	ProbFunc::generator.seed(time(0));
	monteCarlo();
	anneal();		
}

void Threaded::stop(){
	//state->patterns.write(state->analysis + "patterns.csv");
	//state->coefficients.write(state->analysis + "coefficients.csv");

	//ofstream fout;
	//fout.open(state->analysis + "expression.txt");
	//fout << state->coefficients.matrix*state->patterns.matrix;
	//fout.close();
}

