#include "MonteDebug.h"

MonteDebug::MonteDebug(State* st): MonteAnneal(st){
	program = "MonteDebug";
}

double MonteDebug::monteCarlo(){
	callback->monteStartCallback();
	for(int i =0; i < state->MAX_RUNS; i++){
		if(i % state->interruptRuns == 0 && callback != NULL){
			callback->monteCallback(i);
		}
		if(i % state->printRuns == 0 && callback != NULL){
          		callback->montePrintCallback(i);
		}
	}
	if(callback != NULL){
		callback->monteFinalCallback();
	}
	return 1.0;
}

double MonteDebug::anneal(){
	callback->annealStartCallback();
	for(int i = 0; i < 2*state->MAX_RUNS; i++){
		if(i % state->interruptRuns == 0 && callback != NULL){
			bool running = callback->annealCallback(i);
		}
		if(i % state->printRuns == 0 && callback != NULL){
          		callback->annealPrintCallback(i);
		}
	}
	if(callback != NULL){
		callback->annealFinalCallback();
	}
	return 1.0;
}
