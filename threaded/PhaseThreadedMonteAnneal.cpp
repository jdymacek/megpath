//Threaded Monte Anneal functions
//Dakota Martin
//Julian Dymacek
//Created on 5/22/2018
//Modified on 5/30/2019

#include "PhaseThreadedMonteAnneal.h"

PhaseThreadedMonteAnneal::PhaseThreadedMonteAnneal(State* st,int nt): MonteAnneal(st){
	numThreads = nt;
	random_device rd;
	ProbFunc::generator.seed(rd());
	uniform = new UniformPF();
	barrier = new Barrier(numThreads);
	callback = NULL;
	program = "PhaseThreadedMonteAnneal_" + to_string(nt) + "t";
}

/*Run a monte carlo markov chain*/
void PhaseThreadedMonteAnneal::monteCarloThreadCoefficient(){
	callback->monteStartCallback();
	ErrorFunctionRow efRow(state);

	//For each spot take a gamble and record outcome
	Range r = state->getRange(threadMap[this_thread::get_id()]);
	for(int i =0; i < state->MAX_RUNS; i++){
		monteCarloStep(state->coefficients,&efRow,0,state->coefficients.columns(),r.rowStart,r.rowEnd);
		barrier->Wait();
		//wait for Pattern thread to Exchange the coefficients/Patterns
		barrier->Wait();
		if(this_thread::get_id() == rootId){
                        if(i % state->interruptRuns == 0 && callback != NULL){
                                callback->monteCallback(i);
                        }
                        if(i % state->printRuns == 0 && callback != NULL){
                                callback->montePrintCallback(i);
                        }
                }
	}
	if(this_thread::get_id() == rootId && callback != NULL){
		callback->monteFinalCallback();
	}
}
void PhaseThreadedMonteAnneal::monteCarloThreadPattern(){
	callback->monteStartCallback();
	ErrorFunctionCol efCol(dupe);

	//For each spot take a gamble and record outcome
	for(int i =0; i < state->MAX_RUNS; i++){
		if(state->both && i != 0){
			monteCarloStep(dupe->patterns,&efCol);
		}
		barrier->Wait();
		dupe->coefficients.matrix = state->coefficients.matrix;
		state->patterns.matrix = dupe->patterns.matrix;
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

double PhaseThreadedMonteAnneal::monteCarlo(){
	dupe = new State();
	*dupe = *state;
	vector<thread> threads;

	threads.push_back(thread(&PhaseThreadedMonteAnneal::monteCarloThreadPattern,this));		
	//	vector<vector<int>> ranges = state->splitRanges(numThreads-1);
	state->dist = to_string(numThreads-1) + "*1";	
	dupe->dist = to_string(numThreads-1) + "*1";	
	rootId = threads[0].get_id();
	for(int i = 0; i < numThreads-1; ++i){
		Range r = state->getRange(i);
		threads.push_back(thread(&PhaseThreadedMonteAnneal::monteCarloThreadCoefficient,this));
	}


	for(int i =0; i < threads.size();++i){
		threads[i].join();
	}
	ErrorFunctionRow efRow(state);
	return efRow.error();
}


void PhaseThreadedMonteAnneal::annealThreadCoefficient(double t, double alpha){
	callback->annealStartCallback();
	ErrorFunctionRow efRow(state);
	barrier->Wait();

	//For each spot take a gamble and record outcome
	Range r = state->getRange(threadMap[this_thread::get_id()]);
	for(int i =0; i < 2*state->MAX_RUNS; i++){
		annealStep(state->coefficients,t,&efRow,0,state->coefficients.columns(),r.rowStart,r.rowEnd);
		barrier->Wait();
		//wait for Pattern thread to exchange coefficients/rows
		barrier->Wait(); 
		if(i % state->interruptRuns == 0 && callback != NULL){
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


		t *= alpha;
	}
	if(this_thread::get_id() == rootId && callback != NULL){
		callback->annealFinalCallback();
	}
}
void PhaseThreadedMonteAnneal::annealThreadPattern(double t, double alpha){
	callback->annealStartCallback();
	ErrorFunctionCol efCol(dupe);
	barrier->Wait();

	//For each spot take a gamble and record outcome
	for(int i =0; i < 2*state->MAX_RUNS; i++){

		if(state->both && i != 0)
		{		
			annealStep(dupe->patterns,t,&efCol);
		}
		barrier->Wait();
		dupe->coefficients.matrix = state->coefficients.matrix;
		state->patterns.matrix = dupe->patterns.matrix;
		barrier->Wait(); 
		if(i % state->interruptRuns == 0 && callback != NULL){
			barrier->Wait();
			if(this_thread::get_id() == rootId){
				callback->annealCallback(i);
				dupe->patterns.matrix = state->patterns.matrix;
				dupe->coefficients.matrix = state->coefficients.matrix;
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
		t *= alpha;
		//if(i > 1.5*state->MAX_RUNS){
		//	state->both = false;
		//}

	}
	if(this_thread::get_id() == rootId && callback != NULL){
		callback->annealFinalCallback();
	}
}

double PhaseThreadedMonteAnneal::anneal(){


	vector<thread> threads;


	int rowSize = state->coefficients.rows()/numThreads;
	int colSize = state->patterns.columns()/numThreads;
	int rowStart = 0;
	int colStart = 0;

	bool constrained = false;

	double t = state->calcT();
	double alpha = state->calcAlpha(t);
	threads.push_back(thread(&PhaseThreadedMonteAnneal::annealThreadPattern,this,t,alpha));
//	vector<vector<int>> ranges = state->splitRanges(numThreads-1);
	state->dist = to_string(numThreads-1) + "*1";	
	dupe->dist = to_string(numThreads-1) + "*1";	
	rootId = threads[0].get_id();
	for(int i = 0; i < numThreads-1; ++i){
		threads.push_back(thread(&PhaseThreadedMonteAnneal::annealThreadCoefficient,this,t,alpha));
	}

	for(int i =0; i < threads.size();++i){
		threads[i].join();
	}

	ErrorFunctionRow efRow(state);
	return efRow.error();
}
