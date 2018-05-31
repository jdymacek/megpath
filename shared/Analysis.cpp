//Analysis.cpp 
//Julian Dymacek
//Created: 6/6/2017
//A base class for different ConNMF algorithms
//Modified: 5/23/2018

#include "Analysis.h"

Analysis::Analysis(){
	state = NULL;
	algorithm = NULL;
}

void Analysis::start(string filename){
	//initialize global variables
	
	state = new State();
	state->load(filename);
	algorithm = new MonteAnneal(state);
    cachedError = 2*state->expression.rows()*state->expression.cols();
}

void Analysis::run(){

}

void Analysis::stop(){

}

void Analysis::monteCallback(int iterations){}

bool Analysis::annealCallback(int iterations){
	bool running = true;
    ErrorFunctionRow ef(state);
    double error = ef.error();
    if(abs(cachedError - error) < 0.005 && error < cachedError)
        running = false;
    cachedError = error;
	return running;
}

void Analysis::montePrintCallback(int iterations){
    //needs time
    ErrorFunctionRow ef(state);
	cachedError = ef.error();
	if(state->debug){
		cout << iterations << "\t Error = " << cachedError << "\t montecarlo" << endl;
	}
}

void Analysis::annealPrintCallback(int iterations){
	//needs time
    ErrorFunctionRow ef(state);
	cachedError = ef.error();
	if(state->debug){
		cout << iterations << "\t Error = " << cachedError << "\t anneal"<< endl;
	}
}

void Analysis::output(){
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
