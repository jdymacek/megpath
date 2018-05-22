//Standard Analysis functions
//Matthew Dyer
//Julian Dymacek
//Created on 6/6/2017
//Last Modified: 6/20/2017

#include "MonteAnneal.h"

MonteAnneal::MonteAnneal(){
	ProbFunc::generator.seed(time(0));
	uniform = new UniformPF();
	program = "MonteAnneal";
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

double MonteAnneal::anneal(){
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

void MonteAnneal::run(){
	//Could put stop watch in here
	ProbFunc::generator.seed(time(0));
	monteCarlo();
	anneal();		
}

void MonteAnneal::stop(){
	//state->patterns.write(state->analysis + "patterns.csv");
	//state->coefficients.write(state->analysis + "coefficients.csv");

	//ofstream fout;
	//fout.open(state->analysis + "expression.txt");
	//fout << state->coefficients.matrix*state->patterns.matrix;
	//fout.close();
}

void MonteAnneal::output(){
	char curTime[20];
	time_t t;
	struct tm *tmp;
	
	t = time(NULL);
	tmp = localtime(&t);
	if(tmp == NULL){
		fprintf(stderr,"%s",strerror(errno));
		exit(-1);
	}

	int timef = strftime(curTime, sizeof(curTime), "%T %m-%d-%Y" ,tmp);
	if(timef == 0){
		fprintf(stderr,"strftime returned 0\n");
		exit(-1);
	}

	string outputDir = state->directory;
	FileUtil::mkPath(outputDir);
	outputDir = outputDir + "/";

	ErrorFunctionRow efRow(state);


	ofstream fout;
	string outputFile = FileUtil::uniqueFile(outputDir + state->analysis + "results.csv");

	//ouput coefficients
	double max = 0;
	double rowError = 0;
	fout.open(outputFile);

	fout << "#Time : " << curTime << endl;
    fout << "#File : " << state->filename << endl;
	fout << "#Number_of_genes : " << state->coefficients.rows << endl;
    fout << "#Program : " << program << endl;
    fout << "#MAX_RUNS : " << state->MAX_RUNS << endl;
    fout << "#Total_error : " << efRow.error() << endl;
    fout << "#Total_running_time : " << state->time << endl;
	for(int i =0; i < state->patterns.rows; ++i){
		fout << "#" << state->patternNames[i] << "," << state->patterns.matrix.row(i) << endl;
	}

	for(int i = 0; i < state->coefficients.rows; ++i){
		MatrixXd errorMatrix;
    	errorMatrix.noalias() = state->expression.row(i);
    	errorMatrix.noalias() -= (state->coefficients.matrix.row(i) * state->patterns.matrix);
		rowError = errorMatrix.cwiseAbs().sum();

		MatrixXd temp = state->coefficients.matrix.row(i);
		max = temp.sum();
		temp = temp/(max);
		if(state->ids.size() > 0){
			fout << state->ids[i] << ",";
		}
		for(int i =0; i < temp.cols(); ++i){
			fout << temp(0,i) << ",";
		}
		fout << rowError  << endl;
	}
	fout.close();
}
