//Main file
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 7/7/2017

#include "PhaseThreadedMonteAnneal.h"
#include "FlipThreadedMonteAnneal.h"
#include "ThreadedMonteAnneal.h"
#include "Analysis.h"
#include "CmdArgs.h"

using namespace std;

int main(int argc, char** argv){
	CmdArgs args(argc,argv);
	string argFile = "";
	args.getAsString("a",argFile,"../testing/testnmf/test_arguments.txt");
	string al = args.findFlag({{"0","t","T","Threaded"},
				    {"1","tf","TF","FlipThreaded"},
				    {"2","tp","TP","PhaseThreaded"}});
	
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
			a->setAlgorithm(new ThreadedMonteAnneal(a->state,nt));
			break;
		case 1:
			a->setAlgorithm(new FlipThreadedMonteAnneal(a->state,nt));
			break;
		case 2:
			a->setAlgorithm(new PhaseThreadedMonteAnneal(a->state,nt));
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

