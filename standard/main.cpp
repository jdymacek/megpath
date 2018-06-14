//Main file for standard
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 5/23/2018

#include "Analysis.h"
using namespace std;

int main(int argc, char** argv){
	if(argc < 2){
		cerr << "Need an argument file!";
		return 0;
	}
	string argFile = argv[1];
	Analysis* a = new Analysis();
	a->setName("Standard");
	a->load(argFile);
	a->start();
	a->run();
	a->output();
	cout << "Total program running time: " << a->state->time << endl;

	delete a;
	return 0;
}

