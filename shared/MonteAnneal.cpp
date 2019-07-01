//Standard Analysis functions
//Matthew Dyer
//Julian Dymacek
//Dakota Martin
//Created on 6/6/2017
//Last Modified: 5/23/2018

#include "MonteAnneal.h"
#include <unistd.h>

MonteAnneal::MonteAnneal(State* st){
	random_device rd;
	ProbFunc::generator.seed(rd());
	uniform = new UniformPF();
	state = st;
	program = "MonteAnneal";
	callback = NULL;
}

void MonteAnneal::setObserver(Observer* obs){
	callback = obs;
}

bool MonteAnneal::accept(double de,double t){
	return de < 0 ||  exp(-de/t) > uniform->random();
}

void MonteAnneal::monteCarloStep(NMFMatrix& m,ErrorFunction* ef){
	Range r;
	r.rowStart = 0;
	r.rowEnd = m.rows()-1;
	r.colStart = 0;
	r.colEnd = m.columns()-1;
	monteCarloStep(m,ef,r);
}

void MonteAnneal::monteCarloStep(NMFMatrix& m,ErrorFunction* ef, Range r){
	double oldError = 0;
	double error = 0; 
	for(int y = r.rowStart; y <= r.rowEnd; ++y){
		for(int x = r.colStart; x <= r.colEnd; ++x){
			ProbFunc* function = m.functions(y,x);
			double r = function->random();
			if(isnan(r)){
				cout << "nan error" << endl;
				cout << function->toString() << endl;
			}
			if(function->size() == 1){
				oldError = ef->fastError(y,x);
				m.matrix(y,x) = r;
				error = ef->fastError(y,x);
			}else{
				oldError = ef->error();
				for(int k=0; k < function->size(); ++k){
					Entry e = function->getEntry(k);
					m.matrix(e.y,e.x) = e.val;
				}
				error = ef->error();
			}
			if(error <= oldError){
				function->addObservation(r);
			}else{
				state->errorAvg += error - oldError;
				state->errorCount++;
			}
		}
	}
}

void MonteAnneal::annealStep(NMFMatrix& m, double t,ErrorFunction* ef){
	Range r;
	r.rowStart = 0;
	r.rowEnd = m.rows()-1;
	r.colStart = 0;
	r.colEnd = m.columns()-1;
	annealStep(m,t,ef,r);
}

void MonteAnneal::annealStep(NMFMatrix& m, double t,ErrorFunction* ef, Range r){
	vector<Entry> entries;
	entries.push_back({0,0,0});
	double olderror = 0;
	double error = 0;

	for(int y = r.rowStart; y <= r.rowEnd; y++){
		for(int x = r.colStart; x <= r.colEnd; x++){

			ProbFunc* function = m.functions(y,x);
			double r = function->random();
			if(function->size() == 1){
				olderror = ef->fastError(y,x);
				entries[0].x = x;
				entries[0].y = y;
				entries[0].val = m.matrix(y,x);
				m.matrix(y,x) = r;
				error = ef->fastError(y,x);
			}else{
				while(entries.size() < function->size()){
					entries.push_back({0,0,0});
				}
				olderror = ef->error();
				for(int k=0; k < function->size(); ++k){
					Entry e = function->getEntry(k);
					double old = m.matrix(e.y,e.x);
					m.matrix(e.y,e.x) = e.val;
					e.val = old;
					entries[k] = e;
				}
				error = ef->error();
			}
			if(!accept(error-olderror,t)){
				for(int i =0; i < function->size(); ++i){
					m.matrix(entries[i].y,entries[i].x) = entries[i].val;
				}
			}else if(error >= olderror){
				state->errorAvg += error - olderror;
				state->errorCount++;
			}else{
//				function->addObservation(r);
			}
		}
	}
}

/*Run a monte carlo markov chain*/
double MonteAnneal::monteCarlo(){
	callback->monteStartCallback();
	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);

	//For each spot take a gamble and record outcome
	for(int i =0; i < state->MAX_RUNS; i++){
		if(state->both){
			monteCarloStep(state->patterns,&efCol);
		}
		monteCarloStep(state->coefficients,&efRow);
		if(i % state->printRuns == 0 && callback != NULL){
        	callback->montePrintCallback(i);
        }
		if(i % state->interruptRuns == 0 && callback != NULL){
			callback->monteCallback(i);
		}
	}
	if(callback != NULL){
		callback->monteFinalCallback();
	}
	return efRow.error();
}

double MonteAnneal::anneal(){
	callback->annealStartCallback();
	int ndx = 0;
	double t = state->calcT();
	double alpha = state->calcAlpha(t);

	state->patterns.transition();
	state->coefficients.transition();

	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);

	bool running = true;
	while(running && ndx < 2*state->MAX_RUNS){
		annealStep(state->coefficients,t,&efRow);
		if(state->both){
			annealStep(state->patterns,t,&efCol);
		}
		if(ndx % state->printRuns == 0 && callback != NULL){
			callback->annealPrintCallback(ndx);
		}
		if(ndx % state->interruptRuns == 0 && callback != NULL){
			running = callback->annealCallback(ndx);
		}
		t *= alpha;
		//0.99975; - the old alpha

		ndx++;
	}
	if(callback != NULL){
		callback->annealFinalCallback();
	}
	
	return efRow.error();
}

