//Threaded Monte Anneal functions
//Dakota Martin
//Julian Dymacek
//Created on 5/22/2018
//Modified on 6/27/2018
//git repository has the thread affinity stuff around 6/27/2018

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
void ThreadedMonteAnneal::monteCarloThread(){
	callback->monteStartCallback();
	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);
	//For each spot take a gamble and record outcome
	Range rc = state->getRange(threadMap[this_thread::get_id()]);
	Range rp = state->getRange(threadMap[this_thread::get_id()]*numThreads);
	for(int i =0; i < state->MAX_RUNS; i++){
		monteCarloStep(state->coefficients,&efRow,0,state->coefficients.columns(),rc.rowStart,rc.rowEnd);
		if(state->both){
			barrier->Wait();
			monteCarloStep(state->patterns,&efCol,rp.colStart,rp.colEnd,0,state->patterns.rows());
		}
		barrier->Wait();
		if(this_thread::get_id() == rootId){
			if(i % state->interruptRuns == 0 && callback != NULL){
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

//	vector<vector<int>> ranges = state->splitRanges(numThreads);
	state->dist = to_string(numThreads) + "*" + to_string(numThreads);
	for(int i = 0; i < numThreads; ++i){	
		//TODO: Constrained rows need reconfiguration
		if(state->constrained){
//			ranges[i][0] = 0;
//			ranges[i][1] = (i == 0 ? state->patterns.columns : 0);
		}
		threads.push_back(thread(&ThreadedMonteAnneal::monteCarloThread,this));
		threadMap[threads[i].get_id()] = i;
		rootId = threads[0].get_id();
	}


	for(int i =0; i < threads.size();++i){
		threads[i].join();
	}

	ErrorFunctionRow efRow(state);
	return efRow.error();
}


void ThreadedMonteAnneal::annealThread(double t, double alpha){
	callback->annealStartCallback();
	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);

	//For each spot take a gamble and record outcome
	Range rc = state->getRange(threadMap[this_thread::get_id()]);
	Range rp = state->getRange(threadMap[this_thread::get_id()]*numThreads);
	for(int i =0; i < 2*state->MAX_RUNS; i++){
		annealStep(state->coefficients,t,&efRow,0,state->coefficients.columns(),rc.rowStart,rc.rowEnd);
		if(state->both){
			barrier->Wait();
			annealStep(state->patterns,t,&efCol,rp.colStart,rp.colEnd,0,state->patterns.rows());
		}
		barrier->Wait(); 
		if(this_thread::get_id() == rootId){
			if(i % state->interruptRuns == 0 && callback != NULL){
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

	double t = state->calcT();
	double alpha = state->calcAlpha(t);
//	vector<vector<int>> ranges = state->splitRanges(numThreads);
	for(int i = 0; i < numThreads; ++i){
		//TODO: Constrained rows need reconfiguration
		if(state->constrained){
//			ranges[i][0] = 0;
//			ranges[i][1] = (i == 0 ? state->patterns.columns : 0);
		}

		threads.push_back(thread(&ThreadedMonteAnneal::annealThread,this,t,alpha));
		rootId = threads[0].get_id();
	}

	for(int i =0; i < threads.size();++i){
		threads[i].join();
	}

	ErrorFunctionRow efRow(state);

	return efRow.error();
}
