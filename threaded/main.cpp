//Main file
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 7/7/2017

#include "Threaded.h"
#include "Stopwatch.h"

using namespace std;

int main(int argc, char** argv){
	if(argc < 3){
		cerr << "Needs an argument file and analysis to run!";
		return 0;
	}
	Stopwatch watch;
	string argFile = argv[1];
	string analysis = argv[2];
	
	Analysis* a;

	if(analysis == "Threaded" || analysis == "T" || analysis == "t"){
		a = new Threaded(5);
	}

	a->start(argFile);

	watch.start();	
	a->run();
	a->state->time = watch.formatTime(watch.stop());
	cout << "Total program running time: " << a->state->time << endl;

	a->stop();

	return 0;
}

