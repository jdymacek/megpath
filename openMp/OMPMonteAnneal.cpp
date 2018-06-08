//OMP Monte Anneal functions
//Dakota Martin
//Julian Dymacek
//Created on 6/7/2018

#include "OMPMonteAnneal.h"

OMPMonteAnneal::OMPMonteAnneal(State* st,int nt): MonteAnneal(st){
	numThreads = nt;
	random_device rd;
	ProbFunc::generator.seed(rd());
	uniform = new UniformPF();
	callback = NULL;
}

/*Run a monte carlo markov chain*/
void OMPMonteAnneal::monteCarloThread(int xStart, int xEnd,int yStart,int yEnd){
	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);
	
	//For each spot take a gamble and record outcome
	for(int i =0; i < state->MAX_RUNS; i++){
		monteCarloStep(state->coefficients,&efRow,0,state->coefficients.columns,yStart,yEnd);
        if(state->both){
		#pragma omp barrier
        	monteCarloStep(state->patterns,&efCol,xStart,xEnd,0,state->patterns.rows);
		#pragma omp barrier
        }
		#pragma omp master
		{
        	if(i % state->printRuns == 0 && callback != NULL){
			callback->montePrintCallback(i);
        	}
			if(i % state->interuptRuns == 0){
				if(callback != NULL){
					callback->monteCallback(i);
				}
			}
		}
		#pragma omp barrier
	}
}


double OMPMonteAnneal::monteCarlo(){
    Stopwatch watch;
	watch.start();
	
	vector<vector<int>> ranges = state->splitRanges(numThreads);

	int id;
	#pragma omp parallel private(id) num_threads(numThreads)
	{
		id = omp_get_thread_num();
		if(state->constrained){
            ranges[id][1] = (id == 0 ? state->patterns.columns : 0);
        }  
		monteCarloThread(ranges[id][0],ranges[id][1],ranges[id][2],ranges[id][3]);
	}

	ErrorFunctionRow efRow(state);
	if(state->debug){
		cout << "Final Error: " << efRow.error() << endl;
    		cout << "Error Histogram: " << efRow.errorDistribution(10) << endl;
    		cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
	}
	return efRow.error();
}


void OMPMonteAnneal::annealThread(int xStart, int xEnd,int yStart,int yEnd){
    ErrorFunctionRow efRow(state);
    ErrorFunctionCol efCol(state);
	double t = 0.5;

    //For each spot take a gamble and record outcome
    for(int i =0; i < 2*state->MAX_RUNS; i++){
		annealStep(state->coefficients,t,&efRow,0,state->coefficients.columns,yStart,yEnd);
        if(state->both){
			#pragma omp barrier
			annealStep(state->patterns,t,&efCol,xStart,xEnd,0,state->patterns.rows);
			#pragma omp barrier
		}
		#pragma omp master
		{
			if(i % state->interuptRuns == 0 && callback != NULL){
				callback->annealCallback(i);
			}

			if(i % state->printRuns == 0 && callback != NULL){
				callback->annealPrintCallback(i);
			}
		}
		#pragma omp barrier
		t *= 0.99975;
    }
}

double OMPMonteAnneal::anneal(){
	Stopwatch watch;
	watch.start();
	
	vector<vector<int>> ranges = state->splitRanges(numThreads);
	int id;
	#pragma omp parallel private(id) num_threads(numThreads)
	{
		id = omp_get_thread_num();
		ranges[id][0] = 0;
		if(state->constrained){
			ranges[id][1] = (id == 0 ? state->patterns.columns : 0);
		}
        annealThread(ranges[id][0],ranges[id][1],ranges[id][2],ranges[id][3]);
	}

	ErrorFunctionRow efRow(state);
	if(state->debug){
		cout << "Final Error: " << efRow.error() << endl;
		cout << "Error Histogram: " << efRow.errorDistribution(10) << endl;
		cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
	}
	return efRow.error();
}

