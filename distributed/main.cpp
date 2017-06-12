//Main file
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 6/9/2017

#include "ParallelPatterns.h"
#include "FuncThrow.h"
#include "DistNaive.h"
#include "Stopwatch.h"

using namespace std;

int main(int argc, char** argv){
	if(argc < 2){
		cerr << "Need an argument file!";
		return 0;
	}
	string argFile = argv[1];
	Analysis* a = new ParallelPatterns();
	a->start(argFile);
	Stopwatch watch;
    watch.start();	
	a->run();
	cout << "Total program running time: " << watch.formatTime(watch.stop()) << endl;
	a->stop();

	return 0;
}

