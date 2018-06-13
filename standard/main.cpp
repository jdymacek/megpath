//Main file for standard
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 5/23/2018

#include "Standard.h"

using namespace std;

int main(int argc, char** argv){
	if(argc < 2){
		cerr << "Need an argument file!";
		return 0;
	}
	string argFile = argv[1];
	Analysis* a = new Analysis();
	a->load(argFile);
	a->start();
	Stopwatch watch;
	watch.start();	
	a->run();
	a->state->time = watch.formatTime(watch.stop());
	cout << "Total program running time: " << a->state->time << endl;
	a->output();

	delete a;
	return 0;
}

