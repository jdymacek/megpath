//Threaded Monte Anneal functions
//Dakota Martin
//Julian Dymacek
//Created on 6/11/2018

#include "FlipOMPMonteAnneal.h"

FlipOMPMonteAnneal::FlipOMPMonteAnneal(State* st,int nt): MonteAnneal(st){
	numThreads = nt;
	random_device rd;
	ProbFunc::generator.seed(rd());
	uniform = new UniformPF();
	callback = NULL;
	dupe = new State();
	*dupe = *st;
}

/*Run a monte carlo markov chain*/
void FlipOMPMonteAnneal::monteCarloThreadCoefficient(){
	ErrorFunctionRow efRow(state);

	//For each spot take a gamble and record outcome
	for(int i = 0; i < state->MAX_RUNS; i++){
		int id = omp_get_thread_num();
		monteCarloStep(state->coefficients,&efRow,0,state->coefficients.columns(),ranges[id][2],ranges[id][3]);
	 	#pragma omp barrier
		//wait for Pattern thread to Exchange the coefficients/Patterns
	 	#pragma omp barrier
		#pragma omp master
		{
			if(i % state->interruptRuns == 0){
				if(callback != NULL){
						callback->monteCallback(i);
				}
			}
			if(i % state->printRuns == 0 && callback != NULL){
				callback->montePrintCallback(i);
			}
		}
	 	#pragma omp barrier
	}
}
void FlipOMPMonteAnneal::monteCarloThreadPattern(){
	ErrorFunctionCol efCol(dupe);

	//For each spot take a gamble and record outcome
	for(int i = 0; i < state->MAX_RUNS; i++){
		if(state->both && i != 0){
			monteCarloStep(dupe->patterns,&efCol);
		}
	 #pragma omp barrier
		dupe->coefficients.matrix = state->coefficients.matrix;
		state->patterns.matrix = dupe->patterns.matrix;
	 #pragma omp barrier
		#pragma omp master
		{
			if(i % state->interruptRuns == 0){
				if(callback != NULL){
						callback->monteCallback(i);
						dupe->patterns.matrix = state->patterns.matrix;
						dupe->coefficients.matrix = state->coefficients.matrix;
				}
			}
			if(i % state->printRuns == 0 && callback != NULL){
				callback->montePrintCallback(i);
			}
		}
	 	#pragma omp barrier
	}
}

double FlipOMPMonteAnneal::monteCarlo(){
    Stopwatch watch;
    watch.start();
	vector<thread> threads;
	
	ranges = state->splitRanges(numThreads-1);
   	
	int id;
	int max;
    	#pragma omp parallel private(id,max) num_threads(numThreads)	
    	{
		id = omp_get_thread_num();
		max = omp_get_num_threads()-1;
		if(id != max){
			monteCarloThreadCoefficient();
		}else{
			monteCarloThreadPattern();
		}
	}
	ErrorFunctionRow efRow(state);


	if(state->debug){
		cout << "Final Error: " << efRow.error() << endl;
    		cout << "Error Histogram: " << efRow.errorDistribution(10) << endl;
    		cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
	}
	return efRow.error();
}


void FlipOMPMonteAnneal::annealThreadCoefficient(int num, double tstart, double alpha){
	 double t = tstart;
   	 ErrorFunctionRow efRow(state);
	 #pragma omp barrier
	 int id = omp_get_thread_num();

    //For each spot take a gamble and record outcome
    for(int i =num; i < 2*state->MAX_RUNS; i++){
	
	annealStep(state->coefficients,t,&efRow,0,state->coefficients.columns(),ranges[id][2],ranges[id][3]);
	#pragma omp barrier
	    //wait for Pattern thread to exchange coefficients/rows
	#pragma omp barrier
	
	#pragma omp master
	{
        	if(i % state->interruptRuns == 0 && callback != NULL){
			callback->annealCallback(i);
        	}
		if(i % state->printRuns == 0 && callback != NULL){
			callback->annealPrintCallback(i);
		}
	}

	#pragma omp barrier
	//pattern thread switches over to coefficient
	#pragma omp barrier
		

		t *= alpha;
    }
}
void FlipOMPMonteAnneal::annealThreadPattern(){
	double t = state->calcT();
	double alpha = state->calcAlpha(t);
    	ErrorFunctionCol efCol(dupe);
	#pragma omp barrier

    //For each spot take a gamble and record outcome
    for(int i = 0; i < 2*state->MAX_RUNS; i++){
	
    	if(state->both && i != 0)
	{		
	    annealStep(dupe->patterns,t,&efCol);
	}
	#pragma omp barrier
         	dupe->coefficients.matrix = state->coefficients.matrix;
		state->patterns.matrix = dupe->patterns.matrix;
	#pragma omp barrier
	#pragma omp master 
	{
		if(i % state->interruptRuns == 0 && callback != NULL){
			callback->annealCallback(i);
			dupe->patterns.matrix = state->patterns.matrix;
			dupe->coefficients.matrix = state->coefficients.matrix;
        	}
		if(i % state->printRuns == 0 && callback != NULL){
			callback->annealPrintCallback(i);
		}
	}
	//could have given this function less RUNS
	//however that could cause some raceconditions
	//fixing those would cause the barriers to go out of sync
	//this keeps the barriers in sync and prevents race conditions
	#pragma omp barrier
	if(i > 1.5*state->MAX_RUNS){
		state->both = false;
		ranges = state->splitRanges(numThreads);
		annealThreadCoefficient(i+1, t, alpha);
		return;
	}
	#pragma omp barrier

	t *= alpha;
    }
}

double FlipOMPMonteAnneal::anneal(){
	Stopwatch watch;
	watch.start();


    vector<thread> threads;


    int rowSize = state->coefficients.rows()/numThreads;
    int colSize = state->patterns.columns()/numThreads;
    int rowStart = 0;
    int colStart = 0;
	
    bool constrained = false;

    double t = state->calcT();
    double alpha = state->calcAlpha(t);
    ranges = state->splitRanges(numThreads-1);
	
    int id;
    int max;
    #pragma omp parallel private(id, max) num_threads(numThreads)	
    {
	    id = omp_get_thread_num();
	    max = omp_get_num_threads()-1;
	    if(id != max){
	    	annealThreadCoefficient(0, t,alpha);
	    }else{
		annealThreadPattern();
	    }
    }


	ErrorFunctionRow efRow(state);
	if(state->debug){
		cout << "Final Error: " << efRow.error() << endl;
		cout << "Error Histogram: " << efRow.errorDistribution(10) << endl;
		cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
	}
	return efRow.error();
}

