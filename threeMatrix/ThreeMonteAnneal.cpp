//Standard Analysis functions
//Dakota Martin
//Created on 6/22/2018

#include "ThreeMonteAnneal.h"
#include <unistd.h>

ThreeMonteAnneal::ThreeMonteAnneal(ThreeState* st):MonteAnneal(st){
	state = st;
}
/*Run a monte carlo markov chain*/
double ThreeMonteAnneal::monteCarlo(){
	callback->monteStartCallback();
	ThreeErrorFunction ef(state);

	//For each spot take a gamble and record outcome
	for(int i =0; i < state->MAX_RUNS; i++){
		if(state->both){
			monteCarloStep(state->patterns,&ef);
		}
		monteCarloStep(state->coefficients,&ef);
		monteCarloStep(state->weights, &ef);
		if(i % state->interuptRuns == 0 && callback != NULL){
			callback->monteCallback(i);
		}
		if(i % state->printRuns == 0 && callback != NULL){
            		callback->montePrintCallback(i);
        	}

	}
	if(callback != NULL){
		callback->monteFinalCallback();
	}
	return ef.error();
}

double ThreeMonteAnneal::anneal(){
	callback->annealStartCallback();
	int ndx = 0;
	double t = state->calcT();
	double alpha = state->calcAlpha(t);

	ThreeErrorFunction ef(state);

	bool running = true;
	while(running && ndx < 2*state->MAX_RUNS){
		annealStep(state->coefficients,t,&ef);
		if(state->both){
			annealStep(state->patterns,t,&ef);
		}
		annealStep(state->weights,t,&ef);
		if(ndx % state->interuptRuns == 0 && callback != NULL){
			running = callback->annealCallback(ndx);
		}
		if(ndx % state->printRuns == 0 && callback != NULL){
			callback->annealPrintCallback(ndx);
		}
		t *= alpha;
		//0.99975; - the old alpha

		ndx++;
	}
	if(callback != NULL){
		callback->annealFinalCallback();
	}
	
	return ef.error();
}

