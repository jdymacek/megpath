//Main file for standard
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 5/23/2018

#include "Analysis.h"
#include "CmdArgs.h"
using namespace std;

int main(int argc, char** argv){
	CmdArgs args(argc,argv);
	string argFile = "";
	args.getAsString("a",argFile,"../testing/testnmf/test_arguments.txt");
	if(args.wasFatal()){
		cout << "Missing Args" << endl;
		cout << args.errors() << endl;
		exit(1);
	}
	Analysis* a = new Analysis();
	a->load(argFile);
	int runTime = args.getAsInt("rt", "-1");
	if(runTime != -1){
		a->timedRun(runTime);
	}
	int runs = args.getAsInt("r","1");
	for(int i = 0; i < runs; i++)
	{
		a->start();
		a->run();
		a->output();
	}
	cout << "Total program running time: " << a->ttime << endl;

	delete a;
	return 0;
}

