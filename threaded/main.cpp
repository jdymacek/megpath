//Main file
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 7/7/2017

#include "PhaseThreaded.h"
#include "Threaded.h"
#include "Stopwatch.h"

using namespace std;

int main(int argc, char** argv){
	if(argc < 3){
		cerr << "Needs: argument file, algorithm";
		return 0;
	}
	Stopwatch watch;
	string argFile = argv[1];
	string analysis = argv[2];
	int nt = thread::hardware_concurrency();
	if(argc == 4){
		nt = atoi(argv[3]);
	}
	Analysis* a;

	if(analysis == "Threaded" || analysis == "T" || analysis == "t"){
			a = new Threaded(nt);
	}else if(analysis == "PhaseThreaded" || analysis == "PT" || analysis == "pt"){
			a = new PhaseThreaded(nt);
	}

	a->start(argFile);

	watch.start();	
	a->run();
	a->state->time = watch.formatTime(watch.stop());
	cout << "Total program running time: " << a->state->time << endl;
	a->output();
	a->stop();

	return 0;
}

