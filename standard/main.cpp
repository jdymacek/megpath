//Main file for standard
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 6/19/2017

#include "MonteAnneal.h"

using namespace std;

int main(int argc, char** argv){
	if(argc < 2){
		cerr << "Need an argument file!";
		return 0;
	}
	string argFile = argv[1];
	Analysis* a = new MonteAnneal();
	a->start(argFile);
	Stopwatch watch;
	watch.start();	
	a->run();
	cout << "Total program running time: " << watch.formatTime(watch.stop()) << endl;
	a->output();

	char bell = 7;
	cout << bell;

	delete a;
	return 0;
}

