//Threaded Monte Anneal functions
//Dakota Martin
//Julian Dymacek
//Created on 5/22/2018
//Modified on 5/23/2018

#include "ThreadedMonteAnneal.h"

ThreadedMonteAnneal::ThreadedMonteAnneal(State* st,int nt): MonteAnneal(st){
	numThreads = nt;
	random_device rd;
	ProbFunc::generator.seed(rd());
	uniform = new UniformPF();
	barrier = new Barrier(numThreads);
	callback = NULL;
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
		if(i % state->printRuns == 0 && callback != NULL){
	        	barrier->Wait();
            		if(this_thread::get_id() == rootId){
				callback->montePrintCallback(i);
            		}
			barrier->Wait();
		}
	}
}


double ThreadedMonteAnneal::monteCarlo(){
    	Stopwatch watch;
	watch.start();
	vector<thread> threads;
	

	vector<vector<int>> ranges = state->splitRanges(numThreads);
	for(int i = 0; i < ranges.size(); ++i){
	
		if(state->constrained == true && i == 0){
			ranges[i][0] = 0;
			ranges[i][1] = state->patterns.columns;
		}
		else if(state->constrained == true){
			ranges[i][0] = 0;
			ranges[i][1] = 0;
		}
        	threads.push_back(thread(&ThreadedMonteAnneal::monteCarloThread,this,ranges[i][0],ranges[i][1],ranges[i][2],ranges[i][3]));
		rootId = threads[0].get_id();
	}


	for(int i =0; i < threads.size();++i){
		threads[i].join();
	}

	ErrorFunctionRow efRow(state);
	if(state->debug){
		cout << "Final Error: " << efRow.error() << endl;
    		cout << "Error Histogram: " << efRow.errorDistribution(10) << endl;
    		cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
	}
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
		//if(i > 1.5*state->MAX_RUNS){
		//	state->both = false;
		//}
    }
}

double ThreadedMonteAnneal::anneal(){
	Stopwatch watch;
	watch.start();


    vector<thread> threads;
	
    bool constrained = false;

    

	vector<vector<int>> ranges = state->splitRanges(numThreads); 
	for(int i = 0; i < ranges.size(); ++i){
	
		if(state->constrained == true && i == 0){
			ranges[i][0] = 0;
			ranges[i][1] = state->patterns.columns;
		}
		else if(state->constrained == true){
			ranges[i][0] = 0;
			ranges[i][1] = 0;
		}
        	threads.push_back(thread(&ThreadedMonteAnneal::annealThread,this,ranges[i][0],ranges[i][1],ranges[i][2],ranges[i][3]));
		rootId = threads[0].get_id();
	}

    for(int i =0; i < threads.size();++i){
        threads[i].join();
    }



	ErrorFunctionRow efRow(state);
	if(state->debug){
		cout << "Final Error: " << efRow.error() << endl;
		cout << "Error Histogram: " << efRow.errorDistribution(10) << endl;
		cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
	}
	return efRow.error();
}

