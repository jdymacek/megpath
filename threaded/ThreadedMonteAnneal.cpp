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
	program = "ThreadedMonteAnneal_" + to_string(nt) + "t";
	callback = NULL;
}

/*Run a monte carlo markov chain*/
void ThreadedMonteAnneal::monteCarloThread(int xStart, int xEnd,int yStart,int yEnd){
	callback->monteStartCallback();
	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);
	//For each spot take a gamble and record outcome
	for(int i =0; i < state->MAX_RUNS; i++){
		monteCarloStep(state->coefficients,&efRow,0,state->coefficients.columns,yStart,yEnd);
        if(state->both){
            barrier->Wait();
            monteCarloStep(state->patterns,&efCol,xStart,xEnd,0,state->patterns.rows);
        }
        barrier->Wait();
		if(this_thread::get_id() == rootId){
			if(i % state->interuptRuns == 0 && callback != NULL){
				callback->monteCallback(i);
			}
			if(i % state->printRuns == 0 && callback != NULL){
				callback->montePrintCallback(i);
            		}
		}
		barrier->Wait();
	}
	if(this_thread::get_id() == rootId && callback != NULL){
		callback->monteFinalCallback();
	}
}


double ThreadedMonteAnneal::monteCarlo(){
	vector<thread> threads;
	

	bool combine = true;
	int toSplit = thread::hardware_concurrency();
	if(toSplit < numThreads){
		toSplit = numThreads;
		combine = false;
	}
	vector<vector<int>> ranges = state->splitRanges(toSplit);
	for(int i = 0; i < ranges.size(); ++i){
	
		if(state->constrained){
			ranges[i][0] = 0;
			ranges[i][1] = (i == 0 ? state->patterns.columns : 0);
		}
		
		if(combine == true){
			int next = i+(toSplit/numThreads);
			if(toSplit % numThreads == 0){
				next--;
			}
			if(state->constrained && i == 0){
				ranges[next][1] = state->patterns.columns;
			}
        		threads.push_back(thread(&ThreadedMonteAnneal::monteCarloThread,this,ranges[i][0],ranges[next][1],ranges[i][2],ranges[next][3]));
			i = next;
			if(i > toSplit%numThreads && toSplit%numThreads != 0){
				combine = false;
			}
		}else{
			threads.push_back(thread(&ThreadedMonteAnneal::monteCarloThread,this,ranges[i][0],ranges[i][1],ranges[i][2],ranges[i][3]));
		}
		rootId = threads[0].get_id();
	}


	for(int i =0; i < threads.size();++i){
		threads[i].join();
	}

    	ErrorFunctionRow efRow(state);
	return efRow.error();
}


void ThreadedMonteAnneal::annealThread(int xStart, int xEnd,int yStart,int yEnd){
    	callback->annealStartCallback();
	ErrorFunctionRow efRow(state);
    	ErrorFunctionCol efCol(state);
	double t = state->calcT();
	double alpha = state->calcAlpha(t);

    	//For each spot take a gamble and record outcome
    	for(int i =0; i < 2*state->MAX_RUNS; i++){
		annealStep(state->coefficients,t,&efRow,0,state->coefficients.columns,yStart,yEnd);
        	if(state->both){
        	    barrier->Wait();
			annealStep(state->patterns,t,&efCol,xStart,xEnd,0,state->patterns.rows);
		}
        	barrier->Wait(); 
		if(this_thread::get_id() == rootId){
			if(i % state->interuptRuns == 0 && callback != NULL){
				callback->annealCallback(i);
			}
			if(i % state->printRuns == 0 && callback != NULL){
				callback->annealPrintCallback(i);
            		}
		}
		barrier->Wait();

		t *= alpha;
    	}
	if(this_thread::get_id() == rootId && callback != NULL){
		callback->annealFinalCallback();
	}
}

double ThreadedMonteAnneal::anneal(){


    vector<thread> threads;
	

    
	bool combine = true;
	int toSplit = thread::hardware_concurrency();
	if(toSplit < numThreads){
		toSplit = numThreads;
		combine = false;
	}
	vector<vector<int>> ranges = state->splitRanges(toSplit);
	for(int i = 0; i < ranges.size(); ++i){
	
		if(state->constrained){
			ranges[i][0] = 0;
			ranges[i][1] = (i == 0 ? state->patterns.columns : 0);
		}
		
		if(combine == true){
			int next = i+(toSplit/numThreads);
			if(toSplit % numThreads == 0){
				next--;
			}
			if(state->constrained && i == 0){
				ranges[next][1] = state->patterns.columns;
			}
        		threads.push_back(thread(&ThreadedMonteAnneal::annealThread,this,ranges[i][0],ranges[next][1],ranges[i][2],ranges[next][3]));
			i = next;
			if(i > toSplit%numThreads && toSplit%numThreads != 0){
				combine = false;
			}
		}else{
			threads.push_back(thread(&ThreadedMonteAnneal::annealThread,this,ranges[i][0],ranges[i][1],ranges[i][2],ranges[i][3]));
		}
		rootId = threads[0].get_id();
	}

    for(int i =0; i < threads.size();++i){
        threads[i].join();
    }


    ErrorFunctionRow efRow(state);

	return efRow.error();
}

