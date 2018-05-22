//Standard Analysis functions
//Matthew Dyer
//Julian Dymacek
//Created on 6/6/2017
//Last Modified: 6/20/2017

#include "threaded.h"

Threaded::Threaded(){
	ProbFunc::generator.seed(time(0));
	uniform = new UniformPF();
	program = "Threaded";
}

/*Run a monte carlo markov chain*/
double Threaded::monteCarlo(){
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
		if(i % state->printRuns == 0){
			double error = efRow.error();
			cout << i << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
		}
	}
	cout << "Final Error: " << efRow.error() << endl;
	cout << "Error Histogram: " << efRow.errorDistribution(10) << endl;
	cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
	return efRow.error();
}

double Threaded::anneal(){
	Stopwatch watch;
	int ndx = 0;
	double t = 0.5;
	watch.start();

	ErrorFunctionRow efRow(state);
	ErrorFunctionCol efCol(state);


	double formerError = 2*state->expression.rows()*state->expression.cols();
	bool running = true;
	while(running && ndx < 2*state->MAX_RUNS){
		annealStep(state->coefficients,t,&efRow);
		if(state->both){
			annealStep(state->patterns,t,&efCol);
		}
		if(ndx % state->printRuns == 0){
			double error = efRow.error();
			cout << ndx << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
			if(abs(formerError - error) < 0.005 && error < formerError)
				running = false;
			formerError = error;
		}
		ndx++;
		t *= 0.99975;
	}
	cout << "Final Error: " << efRow.error() << endl;
	cout << "Error Histogram: " << efRow.errorDistribution(10) << endl;
	cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
	return efRow.error();
}

void Threaded::run(){
	//Could put stop watch in here
	ProbFunc::generator.seed(time(0));
	monteCarlo();
	anneal();		
}

void Threaded::stop(){
	//state->patterns.write(state->analysis + "patterns.csv");
	//state->coefficients.write(state->analysis + "coefficients.csv");

	//ofstream fout;
	//fout.open(state->analysis + "expression.txt");
	//fout << state->coefficients.matrix*state->patterns.matrix;
	//fout.close();
}

