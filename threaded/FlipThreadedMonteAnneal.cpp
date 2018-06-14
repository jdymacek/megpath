//Threaded Monte Anneal functions
//Dakota Martin
//Julian Dymacek
//Created on 6/4/2018

#include "FlipThreadedMonteAnneal.h"

FlipThreadedMonteAnneal::FlipThreadedMonteAnneal(State* st,int nt): MonteAnneal(st){
	numThreads = nt;
	random_device rd;
	ProbFunc::generator.seed(rd());
	uniform = new UniformPF();
	barrier = new Barrier(numThreads);
	callback = NULL;
	program = "FlipThreadedMonteAnneal_" + to_string(nt) + "t";
	dupe = new State();
	*dupe = *st;
}

/*Run a monte carlo markov chain*/
void FlipThreadedMonteAnneal::monteCarloThreadCoefficient(){
    Stopwatch watch;
    watch.start();
	ErrorFunctionRow efRow(state);

	//For each spot take a gamble and record outcome
	for(int i = 0; i < state->MAX_RUNS; i++){
		int id = threadMap[this_thread::get_id()];
		monteCarloStep(state->coefficients,&efRow,0,state->coefficients.columns,ranges[id][2],ranges[id][3]);
            barrier->Wait();
		//wait for Pattern thread to Exchange the coefficients/Patterns
            barrier->Wait();
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
	if(this_thread::get_id() == rootId && callback != NULL){
		state->time = watch.formatTime(watch.stop());
		callback->monteFinalCallback();
	}
}
void FlipThreadedMonteAnneal::monteCarloThreadPattern(){
    Stopwatch watch;
    watch.start();
	ErrorFunctionCol efCol(dupe);

	//For each spot take a gamble and record outcome
	for(int i = 0; i < state->MAX_RUNS; i++){
		if(state->both && i != 0){
			monteCarloStep(dupe->patterns,&efCol);
		}
            barrier->Wait();
		dupe->coefficients.matrix = state->coefficients.matrix;
		state->patterns.matrix = dupe->patterns.matrix;
            barrier->Wait();
		if(i % state->interuptRuns == 0){
			if(callback != NULL){
				barrier->Wait();
				if(this_thread::get_id() == rootId){
					callback->monteCallback(i);
					dupe->patterns.matrix = state->patterns.matrix;
					dupe->coefficients.matrix = state->coefficients.matrix;
				}
				barrier->Wait();
			}
		}
	}
	if(this_thread::get_id() == rootId && callback != NULL){
		state->time = watch.formatTime(watch.stop());
		callback->monteFinalCallback();
	}
}

double FlipThreadedMonteAnneal::monteCarlo(){
	vector<thread> threads;
	
	threads.push_back(thread(&FlipThreadedMonteAnneal::monteCarloThreadPattern,this));		
	
	ranges = state->splitRanges(numThreads-1);
	rootId = threads[0].get_id();
   	for(int i = 0; i < ranges.size(); ++i){
   	 	threads.push_back(thread(&FlipThreadedMonteAnneal::monteCarloThreadCoefficient,this));
		threadMap[threads[i+1].get_id()] = i;
	}
	
	
	for(int i =0; i < threads.size();++i){
		threads[i].join();
	}
	ErrorFunctionRow efRow(state);
	return efRow.error();
}


void FlipThreadedMonteAnneal::annealThreadCoefficient(int num, double tstart, double alpha){
	Stopwatch watch;
    watch.start();
	 double t = tstart;
   	 ErrorFunctionRow efRow(state);
   	 barrier->Wait();
	 int id = threadMap[this_thread::get_id()];

    //For each spot take a gamble and record outcome
    for(int i =num; i < 2*state->MAX_RUNS; i++){
		annealStep(state->coefficients,t,&efRow,0,state->coefficients.columns,ranges[id][2],ranges[id][3]);
            barrier->Wait();
	    //wait for Pattern thread to exchange coefficients/rows
	    barrier->Wait(); 
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
		barrier->Wait();
		//pattern thread switches over to coefficient
		barrier->Wait();
		

		t *= alpha;
    }
	if(this_thread::get_id() == rootId && callback != NULL){
		state->time = watch.formatTime(watch.stop());
		callback->annealFinalCallback();
	}
}
void FlipThreadedMonteAnneal::annealThreadPattern(){
	Stopwatch watch;
    watch.start();
	double t = state->calcT();
	double alpha = state->calcAlpha(t);
    	ErrorFunctionCol efCol(dupe);
	barrier->Wait();

    //For each spot take a gamble and record outcome
    for(int i = 0; i < 2*state->MAX_RUNS; i++){
	
    	if(state->both && i != 0)
	{		
	    annealStep(dupe->patterns,t,&efCol);
	}
	    barrier->Wait();
         	dupe->coefficients.matrix = state->coefficients.matrix;
		state->patterns.matrix = dupe->patterns.matrix;
	    barrier->Wait(); 
        if(i % state->interuptRuns == 0 && callback != NULL){
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
	//could have given this function less RUNS
	//however that could cause some raceconditions
	//fixing those would cause the barriers to go out of sync
	//this keeps the barriers in sync and prevents race conditions
	barrier->Wait();
	if(i > 1.5*state->MAX_RUNS){
		state->both = false;
		ranges = state->splitRanges(numThreads);
		threadMap[this_thread::get_id()] = ranges.size()-1;
		annealThreadCoefficient(i+1, t, alpha);
		return;
	}
	barrier->Wait();

	t *= alpha;
    }
	if(this_thread::get_id() == rootId && callback != NULL){
		state->time = watch.formatTime(watch.stop());
		callback->annealFinalCallback();
	}
}

double FlipThreadedMonteAnneal::anneal(){


    vector<thread> threads;


    int rowSize = state->coefficients.rows/numThreads;
    int colSize = state->patterns.columns/numThreads;
    int rowStart = 0;
    int colStart = 0;
	
    bool constrained = false;

    double t = state->calcT();
    double alpha = state->calcAlpha(t);
    threads.push_back(thread(&FlipThreadedMonteAnneal::annealThreadPattern,this));
    ranges = state->splitRanges(numThreads-1);
    rootId = threads[0].get_id();
    for(int i = 0; i < ranges.size(); ++i){
    	threads.push_back(thread(&FlipThreadedMonteAnneal::annealThreadCoefficient,this, 0, t,alpha));
	threadMap[threads[i+1].get_id()] = i;
    }

    for(int i =0; i < threads.size();++i){
        threads[i].join();
    }



	ErrorFunctionRow efRow(state);
	return efRow.error();
}

