//Main file
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 7/7/2017

#include "OMPMonteAnneal.h"
#include "FlipOMPMonteAnneal.h"
#include "Analysis.h"
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
	Analysis* a = new Analysis();

	a->load(argFile);
	if(analysis == "OpenMP" || analysis == "O" || analysis == "o"){
			a->setAlgorithm(new OMPMonteAnneal(a->state, nt));
	}else if(analysis == "FlipOpenMp" || analysis == "FO" || analysis == "fo"){
			a->setAlgorithm(new FlipOMPMonteAnneal(a->state, nt));
	}

	a->start();

	a->run();
	a->output();
	cout << "Total program running time: " << a->state->time << endl;
	a->stop();

	return 0;
}

