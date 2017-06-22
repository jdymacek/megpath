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
	if(argc < 2){
		cerr << "Need an argument file!";
		return 0;
	}
	string argFile = argv[1];
//	Analysis* a = new DistNaive();
//	Analysis* a = new FuncThrow();
//	Analysis* a = new ParallelPatterns();
	Analysis* a = new ParallelFuncThrow();
	a->start(argFile);
	Stopwatch watch;
	watch.start();	
	a->run();
	cout << "Total program running time: " << watch.formatTime(watch.stop()) << endl;
	a->stop();

	return 0;
}

