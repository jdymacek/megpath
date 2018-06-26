//Main file
//Dakota Martin
//Julian Dymacek
//Created on 6/26/2018

#include "OMPMonteAnneal.h"
#include "FlipOMPMonteAnneal.h"
#include "Analysis.h"
#include "CmdArgs.h"

using namespace std;

int main(int argc, char** argv){
	CmdArgs args(argc,argv);
	string argFile = "";
	args.getAsString("a",argFile,"../testing/testnmf/test_arguments.txt");
	string al = args.findFlag({{"0","o","O","OMPMonteAnneal"},
				    {"1","of","OF","FlipOMPMonteAnneal"}});
	
	int nt = args.getAsInt(al,to_string(thread::hardware_concurrency()));
	int runs = args.getAsInt("r","1");
	if(args.wasFatal()){
		cout << "Missing Args" << endl;
		cout << args.errors() << endl;
		exit(1);
	}
	Analysis* a = new Analysis();
	a->load(argFile);
	switch(stoi(al)){
		case 0:
			a->setAlgorithm(new OMPMonteAnneal(a->state,nt));
			break;
		case 1:
			a->setAlgorithm(new FlipOMPMonteAnneal(a->state,nt));
			break;
	}

	int runTime = args.getAsInt("rt", "-1");
	if(runTime != -1){
		a->timedRun(runTime);
	}
	for(int i = 0; i < runs; i++)
	{
		a->start();
		a->run();
		a->output();
		cout << "Total program running time: " << a->ttime << endl;
		if(a->state->debug){
			cout << "Patterns:\n" << a->state->patterns.matrix << endl;
		}
		a->stop();
	}
	
	delete a;
	return 0;
}

