//StandardAnalysis
//Matthew Dyer
//Julian Dymacek
//Created on 6/6/2017

#include "MonteAnneal.h"

bool MonteAnneal::accept(double de,double t){
    return de < 0 ||  exp(-de/t) > uniform->random();
}

void MonteAnneal::monteCarloStep(NMFMatrix& m){
   for(int y =0; y < m.rows; ++y){
        for(int x =0; x < m.columns; ++x){
            ProbFunc* function = m.functions[y][x];
            double oldError = findError(m,y,x);
            double r = function->random();
            if(function->size() == 1){
                m.matrix(y,x) = r;
            }else{
                for(int k=0; k < function->size(); ++k){
                    Entry e = function->getEntry(k);
                    m.matrix(e.y,e.x) = e.val;
                }
            }
            double error = findError(m,y,x);
            if(error <= oldError){
                function->addObservation(r);
            }
        }
    }
}

void MonteAnneal::annealStep(NMFMatrix& m, double t){
    vector<Entry> entries;
    entries.push_back({0,0,0});

    for(int y =0; y < m.rows; y++){
        for(int x =0; x < m.columns; x++){
            double olderror = findError(m,y,x);

            ProbFunc* function = m.functions[y][x];
            double r = function->random();
            if(function->size() == 1){
                entries[0].x = x;
                entries[0].y = y;
                entries[0].val = m.matrix(y,x);
                m.matrix(y,x) = r;
            }else{
                while(entries.size() < function->size()){
                    entries.push_back({0,0,0});
                }
                for(int k=0; k < function->size(); ++k){
                    Entry e = function->getEntry(k);
                    double old = m.matrix(e.y,e.x);
                    m.matrix(e.y,e.x) = e.val;
                    e.val = old;
                    entries[k] = e;
                }
            }

            double error = findError(m,y,x);
            if(!accept(error-olderror,t)){
                for(int i =0; i < function->size(); ++i){
                    m.matrix(entries[i].y,entries[i].x) = entries[i].val;
                }
            }
        }
    }
}

/*Run a monte carlo markov chain*/
void MonteAnneal::monteCarlo(){
	Stopwatch watch;
	watch.start();

	//For each spot take a gamble and record outcome
	for(int i =0; i < state->MAX_RUNS; i++){
		monteCarloStep(state->patterns);
		monteCarloStep(state->coefficients);
		if(i % 1000 == 0){
			double error = findError();
			cout << i << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
		}
	}
	cout << "Final Error: " << findError() << endl;
	cout << "Error Histogram: " << errorDistribution(10) << endl;	
	cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
}

void MonteAnneal::anneal(){
	Stopwatch watch;
	int ndx = 0;
	double t = 0.5;
	watch.start();

	double formerError = 2*start->expression.rows()*start->expression.cols();
	bool running = true;
	while(running && ndx < MAX_RUNS){
		annealStep(start->coefficients,t);
		annealStep(start->patterns,t);
		if(ndx % 1000 == 0){
			double error = findError();
			cout << ndx << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
			if(abs(formerError - error) < 0.005 && error < formerError)
				running = false;
			formerError = error;
		}
		ndx++;
		t *= 0.99975;
	}
	formerError = findError();
	cout << "Final Error: " << formerError << endl;
	cout << "Error Histogram: " << errorDistribution(10) << endl;
	cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
}

void MonteAnneal::run(){
	//Could put stop watch in here
	ProbFunc::generator.seed(time(0));
	monteCarlo();
	anneal();		
}

void MonteAnneal::stop(){
	state->patterns.write(analysis + "patterns.csv");
	state->coefficients.write(analysis + "coefficients.csv");

	ofstream fout;
	fout.open(state->analysis + "expression.txt");
	fout << state->coefficients.matrix*state->patterns.matrix;
	fout.close();
}
