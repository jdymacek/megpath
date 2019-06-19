//Analysis.cpp 
//
//Julian Dymacek
//Created: 6/6/2017
//A base class for different ConNMF algorithms
//Modified: 5/23/2018

#include "Analysis.h"

Analysis::Analysis(){
	state = new State();
	algorithm = NULL;
}

Analysis::~Analysis(){}

void Analysis::timedRun(int runTime){
	string toSet = state->stats;
	state->stats = "none";
	double prevTime;
	int prevRuns;
	Stopwatch watch;
	int running = 1;
	double deltaT = 0;
	int deltI = 0;
	while(running > 0 ){
		watch.start();
		start();
		run();
		stop();
		double time = watch.stop();
		if(time > runTime-0.5 && time < runTime+0.5){
			running = -5;
		}else if (running == 1){
			//running = 2;
			prevRuns = state->MAX_RUNS;
			prevTime = time;
			state->MAX_RUNS = (state->MAX_RUNS * runTime/time);
		}else{
			double deltaT = ((prevTime - time));
			int deltaI = ((prevRuns - state->MAX_RUNS));
			//if(deltaT > 0.2){	
				int nextRuns = (deltaI/deltaT) * runTime;
				prevTime = time;
				prevRuns = state->MAX_RUNS;
				state->MAX_RUNS = nextRuns;
			//}
		}
		running ++;
		if(state->MAX_RUNS > prevRuns-1 && state->MAX_RUNS < prevRuns+1){
			running = -5;
		}
	}
	state->stats = toSet;

}
void Analysis::setAlgorithm(MonteAnneal* al){
	algorithm = al;
	if(program.size() > 0){
		program += "_";
	}
	program += al->program;
}

void Analysis::setState(State* st){
	state = st;
}
void Analysis::load(string filename){
	if(program.size() == 0){
		program = "Standard";
	}
	state->load(filename);
}


void Analysis::start(){
	//initialize global variables
	state->reset();
	watch.start();
	if(algorithm == NULL){
		algorithm = new MonteAnneal(state);
	}
    	cachedError = 2*state->expression.rows()*state->expression.cols();
}


void Analysis::run(){
	algorithm->setObserver(this);
	algorithm->monteCarlo();
	algorithm->anneal();
}

void Analysis::stop(){
}

void Analysis::monteStartCallback(){
	lap.start();
}
void Analysis::annealStartCallback(){
	lap.start();
}

void Analysis::monteCallback(int iterations){}

bool Analysis::annealCallback(int iterations){
	bool running = true;
    ErrorFunctionRow ef(state);
    double error = ef.error();
    if(error < cachedError && (cachedError - error < 0.005)){
        running = false;
	}
    cachedError = error;
	return running;
}
void Analysis::annealFinalCallback(){
	ErrorFunctionRow efRow(state);

	cout << "Final Error: " << efRow.error() << endl;
	cout << "Error Histogram: " << efRow.errorDistribution(10) << endl;
	cout << "Total time: " << lap.formatTime(lap.stop()) << endl;
}
void Analysis::monteFinalCallback(){
	ErrorFunctionRow efRow(state);

	cout << "Final Error: " << efRow.error() << endl;
	cout << "Error Histogram: " << efRow.errorDistribution(10) << endl;
	cout << "Total time: " << lap.formatTime(lap.stop()) << endl;
}

void Analysis::montePrintCallback(int iterations){
    //needs time
    ErrorFunctionRow ef(state);
	cachedError = ef.error();
	cout << "montecarlo\t" << iterations << "\t" << cachedError/state->expression.size() << endl;
}

void Analysis::annealPrintCallback(int iterations){
	//needs time
    ErrorFunctionRow ef(state);
	cachedError = ef.error();
	cout << "anneal\t" << iterations << "\t" << cachedError/state->expression.size() << endl;
}


void Analysis::output(){
	ttime = watch.formatTime(watch.stop());
	if(state->stats == "notAll"){
		outputStats();
	}else if(state->stats == "withRuns"){
		outputRuns();
	}else if(state->stats == "none"){
	
	}else{
		outputAll();
	}
}



void Analysis::outputRuns(){
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
	outputDir += "/";
	ErrorFunctionRow efRow(state);

	ofstream fout;
	string toComp = "#File: ";
	toComp += state->filename + "\n";
	toComp += "#Constrained: ";
	if(state->constrained){
		toComp += "true\n";
	}else{
		toComp += "false\n";
	}

	string outputFile = FileUtil::findMatchingFile(outputDir + state->analysis + "runResults.csv", toComp, 2);
	if(!FileUtil::isFile(outputFile)){
		fout.open(outputFile);
		fout << toComp;
		fout << "#Time: " << curTime << endl;
	}else{
		fout.open(outputFile, ofstream::app);
	}
	fout << program << "," << ttime << "," << efRow.error() << "," << to_string(state->MAX_RUNS) << endl;
	fout.close();

}

void Analysis::outputStats(){
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
	outputDir += "/";
	ErrorFunctionRow efRow(state);

	ofstream fout;
	string toComp = "#File: ";
	toComp += state->filename + "\n";
	toComp += "#Constrained: ";
	if(state->constrained){
		toComp += "true\n";
	}else{
		toComp += "false\n";
	}
	toComp += "#MAX_RUNS: ";
	toComp += to_string(state->MAX_RUNS) + "\n";


	string outputFile = FileUtil::findMatchingFile(outputDir + state->analysis + "statResults.csv", toComp, 3);
	if(!FileUtil::isFile(outputFile)){
		fout.open(outputFile);
		fout << toComp;
		fout << "#Time: " << curTime << endl;
	}else{
		fout.open(outputFile, ofstream::app);
	}
	fout << program << "," << ttime << "," << efRow.error() << endl;
	fout.close();

}

void Analysis::outputAll(){
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
    if(state->directory != ""){
    	outputDir = outputDir + "/";
    }

    ErrorFunctionRow efRow(state);

    ofstream fout;
    string outputFile = FileUtil::uniqueFile(outputDir + state->analysis + "results.csv");

    //ouput coefficients
    fout.open(outputFile);
    
    double max = 0;
    double rowError = 0;

    fout << "#Time : " << curTime << endl;
    fout << "#File : " << state->filename << endl;
    fout << "#Number_of_genes : " << state->coefficients.rows() << endl;
    fout << "#Program : " << program << endl;
    fout << "#MAX_RUNS : " << state->MAX_RUNS << endl;
	fout << "#Interrupt_runs : " << state->interruptRuns << endl;
	fout << "#Num_Patterns : " << state->patterns.rows() << endl;
	fout << "#Distribution : " << state->dist << endl;
    fout << "#Total_error : " << efRow.error() << endl;
    fout << "#Total_running_time : " << ttime << endl;

    if(state->img){
	    state->MXdToPNG(state->patterns.matrix,state->patterns.rows(),state->patterns.columns(),state->gray,"result.png");
    }else{
    	for(int i =0; i < state->patterns.rows(); ++i){
    	    fout << "#" << state->patternNames[i] << "," << state->patterns.matrix.row(i) << endl;
    	}
    }

    for(int i = 0; i < state->coefficients.rows(); ++i){
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
