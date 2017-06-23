//Main file
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 6/9/2017

#include "DistNaive.h"
#include "FuncThrow.h"
#include "ParallelPatterns.h"
#include "ParallelFuncThrow.h"
#include "Stopwatch.h"

using namespace std;

int main(int argc, char** argv){
	if(argc < 3){
		cerr << "Needs an argument file and analysis to run!";
		return 0;
	}
	string argFile = argv[1];
	string analysis = argv[2];
	
	Analysis* a;

	if(analysis == "DistNaive" || analysis == "DN" || analysis == "dn"){
		a = new DistNaive();
	}else if(analysis == "FuncThrow" || analysis == "FT" || analysis == "ft"){
		a = new FuncThrow();
	}else if(analysis == "ParallelPatterns" || analysis == "ParPat" || analysis == "PP" || analysis == "pp"){
		a = new ParallelPatterns();
	}else if(analysis == "ParallelFuncThrow" || analysis == "ParFuncThrow" || analysis == "PFT" || analysis == "pft"){
		a = new ParallelFuncThrow();
	}

	a->start(argFile);
	Stopwatch watch;
	watch.start();	
	a->run();
	cout << "Total program running time: " << watch.formatTime(watch.stop()) << endl;
	a->stop();

	return 0;
}

