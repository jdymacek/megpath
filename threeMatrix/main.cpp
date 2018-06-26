//Main file for standard
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 5/23/2018

#include "ThreeAnalysis.h"
#include "CmdArgs.h"
#include "ThreeMonteAnneal.h"
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
	Analysis* a = new ThreeAnalysis();
	string thr = args.findFlag({"tr", "TR", "ThreeMonteAnneal"});
	if(thr == "tr"){
	//	a->setState(new ThreeState());
		a->setAlgorithm(new ThreeMonteAnneal((ThreeState*)(a->state)));
	}
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
	cout << "Patterns:\n" << a->state->patterns.matrix << endl;
	cout << "Coefficients:\n" << a->state->coefficients.matrix << endl;
	cout << "Weights:\n" << ((ThreeState*)(a->state))->weights.matrix << endl;
	cout << "Total program running time: " << a->ttime << endl;

	delete a;
	return 0;
}

