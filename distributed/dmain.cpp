//Main file
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 6/6/2017

//#define EIGEN_DEFAULT_TO_ROW_MAJOR

#include "DistNaive.h"

using namespace std;

int main(int argc, char** argv){
	if(argc < 2){
		cerr << "Need an argument file!";
		return 0;
	}
	string argFile = argv[1];
	Analysis* a = new DistNaive();
	a->start(argFile);
	a->run();
	a->stop();

	return 0;
}

