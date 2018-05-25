//Threaded Monte Anneal functions
//Dakota Martin
//Julian Dymacek
//Created on 5/22/2018
//Modified on 5/23/2018

#include "ThreadedMonteAnneal.h"

ThreadedMonteAnneal::ThreadedMonteAnneal(State* st,int nt): MonteAnneal(st){
	numThreads = nt;
	ProbFunc::generator.seed(time(0));
	uniform = new UniformPF();
	barrier = new Barrier(numThreads);
}

/*Run a monte carlo markov chain*/
void ThreadedMonteAnneal::monteCarloThread(int xStart, int xEnd,int yStart,int yEnd){
	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);

	//For each spot take a gamble and record outcome
	for(int i =0; i < state->MAX_RUNS; i++){
		monteCarloStep(state->coefficients,&efRow,0,state->coefficients.columns,yStart,yEnd);
        if(state->both){
            barrier->Wait();
            monteCarloStep(state->patterns,&efCol,xStart,xEnd,0,state->patterns.rows);
            barrier->Wait();
        }
		if(i % state->interuptRuns == 0){
			if(callback != NULL){
				barrier->Wait();
				if(this_thread::get_id() == rootId){
					callback->monteCallback(i);
				}
				barrier->Wait();
			}
		}
	}
}


double ThreadedMonteAnneal::monteCarlo(){
    Stopwatch watch;
    watch.start();
	vector<thread> threads;

	int rowSize = state->coefficients.rows/numThreads;
	int colSize = state->patterns.columns/numThreads;
	int rowStart = 0;
	int colStart = 0;
	
	//check to see if the matrix is constrained
	bool constrained = false;
	for(int x = 0; x < state->patterns.columns; x++){
		for(int y = 0; y < state->patterns.rows; y++){
			if(state->patterns.functions(y,x)->size() > 1){
				constrained = true;
			}
		}
	}

	for(int i =0; i < numThreads; ++i){
		int rowEnd = rowStart + rowSize;
		int colEnd = colStart + colSize;
		if(i < state->coefficients.rows%numThreads)
			rowEnd += 1;
		if(i < state->patterns.columns%numThreads)
			colEnd += 1;
		//give the first thread all of the columns if constrained
		if(constrained == true && i == 0){
			colStart = 0;
			colEnd = state->patterns.columns;
		}
		else if(constrained == true){
			colStart = 0;
			colEnd = 0;
		}
			
		cout << "rs:" << rowStart << " re:" << rowEnd << endl << "cs:" << colStart << " ce:" << colEnd << endl << endl;
		threads.push_back(thread(&ThreadedMonteAnneal::monteCarloThread,this,colStart,colEnd,rowStart,rowEnd));
		rootId = threads[0].get_id();
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


void ThreadedMonteAnneal::annealThread(int xStart, int xEnd,int yStart,int yEnd){
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
        if(i % state->interuptRuns == 0 && callback != NULL){
            barrier->Wait();
            if(this_thread::get_id() == rootId){
				callback->annealCallback(i);
            }
            barrier->Wait();
        }
		if(i % state->printRuns == 0 && callback != NULL){
	        barrier->Wait();
            if(this_thread::get_id() == rootId){
				callback->annealPrintCallback(i);
            }
			barrier->Wait();
		}

		t *= 0.99975;
    }
}

double ThreadedMonteAnneal::anneal(){
	Stopwatch watch;
	watch.start();


    vector<thread> threads;


    int rowSize = state->coefficients.rows/numThreads;
    int colSize = state->patterns.columns/numThreads;
    int rowStart = 0;
    int colStart = 0;
	
    bool constrained = false;

    //check to see if its constrained    
    for(int x = 0; x < state->patterns.columns; x++){
		for(int y = 0; y < state->patterns.rows; y++){
			if(state->patterns.functions(y,x)->size() > 1){
				constrained = true;
			}
		}
    }

    for(int i =0; i < numThreads; ++i){
        int rowEnd = rowStart + rowSize;
        int colEnd = colStart + colSize;
        if(i < state->coefficients.rows%numThreads)
            rowEnd += 1;
        if(i < state->patterns.columns%numThreads)
            colEnd += 1;
		
	//give the first thread all of the columns if constrained
	if(constrained == true && i == 0){
		colStart = 0;
		colEnd = state->patterns.columns;
	}
	else if(constrained == true){
		colStart = 0;
		colEnd = 0;
	}

        threads.push_back(thread(&ThreadedMonteAnneal::annealThread,this,colStart,colEnd,rowStart,rowEnd));
		rootId = threads[0].get_id();
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

