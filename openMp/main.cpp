//Main file
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 7/7/2017

#include "OpenMP.h"
#include "Stopwatch.h"
#include <omp.h>

using namespace std;

int main(int argc, char** argv){
	if(argc < 3){
		cerr << "Needs: argument file, algorithm";
		return 0;
	}
	Stopwatch watch;
	string argFile = argv[1];
	string analysis = argv[2];
	int nt = omp_get_num_threads();
	if(argc == 4){
		nt = atoi(argv[3]);
	}
	Analysis* a;

	if(analysis == "OpenMP" || analysis == "o" || analysis == "o"){
			a = new OpenMP(nt);
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

