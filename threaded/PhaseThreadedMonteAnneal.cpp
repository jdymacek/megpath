//Threaded Monte Anneal functions
//Dakota Martin
//Julian Dymacek
//Created on 5/22/2018
//Modified on 5/23/2018

#include "PhaseThreadedMonteAnneal.h"

PhaseThreadedMonteAnneal::PhaseThreadedMonteAnneal(State* st,int nt): MonteAnneal(st){
	numThreads = nt;
	random_device rd;
	ProbFunc::generator.seed(rd());
	uniform = new UniformPF();
	barrier = new Barrier(numThreads);
	callback = NULL;
	program = "PhaseThreadedMonteAnneal_" + to_string(nt) + "t";
	dupe = new State();
	*dupe = *st;
}

/*Run a monte carlo markov chain*/
void PhaseThreadedMonteAnneal::monteCarloThreadCoefficient(int Start, int End){
	callback->monteStartCallback();
	ErrorFunctionRow efRow(state);

	//For each spot take a gamble and record outcome
	for(int i =0; i < state->MAX_RUNS; i++){
		monteCarloStep(state->coefficients,&efRow,0,state->coefficients.columns,Start,End);
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
		callback->monteFinalCallback();
	}
}

double PhaseThreadedMonteAnneal::monteCarlo(){
	vector<thread> threads;
	
	threads.push_back(thread(&PhaseThreadedMonteAnneal::monteCarloThreadPattern,this));		
	vector<vector<int>> ranges = state->splitRanges(numThreads-1);
	rootId = threads[0].get_id();
   	for(int i = 0; i < ranges.size(); ++i){
		threads.push_back(thread(&PhaseThreadedMonteAnneal::monteCarloThreadCoefficient,this,ranges[i][2],ranges[i][3]));
	}
	
	
	for(int i =0; i < threads.size();++i){
		threads[i].join();
	}
	ErrorFunctionRow efRow(state);
	return efRow.error();
}


void PhaseThreadedMonteAnneal::annealThreadCoefficient(int Start, int End){
	callback->annealStartCallback();
	double t = state->calcT();
	double alpha = state->calcAlpha(t);
    ErrorFunctionRow efRow(state);

    //For each spot take a gamble and record outcome
    for(int i =0; i < 2*state->MAX_RUNS; i++){
		annealStep(state->coefficients,t,&efRow,0,state->coefficients.columns,Start,End);
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
		

		t *= alpha;
    }
	if(this_thread::get_id() == rootId && callback != NULL){
		callback->annealFinalCallback();
	}
}
void PhaseThreadedMonteAnneal::annealThreadPattern(){
	callback->annealStartCallback();
	double t = state->calcT();
	double alpha = state->calcAlpha(t);
    	ErrorFunctionCol efCol(dupe);

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


    int rowSize = state->coefficients.rows/numThreads;
    int colSize = state->patterns.columns/numThreads;
    int rowStart = 0;
    int colStart = 0;
	
    bool constrained = false;


    threads.push_back(thread(&PhaseThreadedMonteAnneal::annealThreadPattern,this));
    vector<vector<int>> ranges = state->splitRanges(numThreads-1);
    rootId = threads[0].get_id();
    for(int i = 0; i < ranges.size(); ++i){
    	threads.push_back(thread(&PhaseThreadedMonteAnneal::annealThreadCoefficient,this,ranges[i][2],ranges[i][3]));
    }

    for(int i =0; i < threads.size();++i){
        threads[i].join();
    }



	ErrorFunctionRow efRow(state);
	return efRow.error();
}

