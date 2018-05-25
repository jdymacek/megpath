//Standard Analysis functions
//Matthew Dyer
//Julian Dymacek
//Dakota Martin
//Created on 6/6/2017
//Last Modified: 5/23/2018

#include "MonteAnneal.h"

MonteAnneal::MonteAnneal(State* st){
	ProbFunc::generator.seed(time(0));
	uniform = new UniformPF();
	state = st;
	callback = NULL;
}

void MonteAnneal::setObserver(Observer* obs){
	callback = obs;
}

bool MonteAnneal::accept(double de,double t){
	return de < 0 ||  exp(-de/t) > uniform->random();
}

void MonteAnneal::monteCarloStep(NMFMatrix& m,ErrorFunction* ef){
	monteCarloStep(m,ef,0,m.columns,0,m.rows);
}

void MonteAnneal::monteCarloStep(NMFMatrix& m,ErrorFunction* ef, int xStart, int xStop, int yStart, int yStop){
	double oldError = 0;
	double error = 0; 
	for(int y = yStart; y < yStop; ++y){
		for(int x = xStart; x < xStop; ++x){
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
			}
		}
	}
}

void MonteAnneal::annealStep(NMFMatrix& m, double t,ErrorFunction* ef){
	annealStep(m,t,ef,0,m.columns,0,m.rows);
}

void MonteAnneal::annealStep(NMFMatrix& m, double t,ErrorFunction* ef, int xStart, int xStop, int yStart, int yStop){
	vector<Entry> entries;
	entries.push_back({0,0,0});
	double olderror = 0;
	double error = 0;

	for(int y =yStart; y < yStop; y++){
		for(int x = xStart; x < xStop; x++){

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
			}
		}
	}
}

/*Run a monte carlo markov chain*/
double MonteAnneal::monteCarlo(){
	Stopwatch watch;
	watch.start();
	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);

	//For each spot take a gamble and record outcome
	for(int i =0; i < state->MAX_RUNS; i++){
		if(state->both){
			monteCarloStep(state->patterns,&efCol);
		}
		monteCarloStep(state->coefficients,&efRow);
		if(i % state->interuptRuns == 0 && callback != NULL){
			callback->monteCallback(i);
		}
		if(i % state->printRuns == 0 && callback != NULL){
            callback->montePrintCallback(i);
        }

	}
	cout << "Final Error: " << efRow.error() << endl;
	cout << "Error Histogram: " << efRow.errorDistribution(10) << endl;
	cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
	return efRow.error();
}

double MonteAnneal::anneal(){
	Stopwatch watch;
	int ndx = 0;
	double t = 0.5;
	watch.start();

	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);

	bool running = true;
	while(running && ndx < 2*state->MAX_RUNS){
		annealStep(state->coefficients,t,&efRow);
		if(state->both){
			annealStep(state->patterns,t,&efCol);
		}
		if(ndx % state->interuptRuns == 0 && callback != NULL){
			running = callback->annealCallback(ndx);
		}
		if(ndx % state->printRuns == 0 && callback != NULL){
			callback->annealPrintCallback(ndx);
		}

		ndx++;
		t *= 0.99975;
	}
	cout << "Final Error: " << efRow.error() << endl;
	cout << "Error Histogram: " << efRow.errorDistribution(10) << endl;
	cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
	return efRow.error();
}

