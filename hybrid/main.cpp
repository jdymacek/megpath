//Main file
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 7/7/2017

#include "PhaseThreadedMonteAnneal.h"
#include "FlipThreadedMonteAnneal.h"
#include "ThreadedMonteAnneal.h"
#include "DistNaive.h"
#include "FuncThrow.h"
#include "ParallelPatterns.h"
#include "ParallelFuncThrow.h"
#include "PatternMatching.h"
#include "CmdArgs.h"

using namespace std;

int main(int argc, char** argv){
	CmdArgs args(argc,argv);
	string argFile = "";
	args.getAsString("a",argFile,"../testing/testnmf/test_arguments.txt");
	string analysis = args.findFlag({{"0","dn", "DN", "DistNaive"},
					  {"1","ft", "FT", "Functhrow"},
					  {"2","pp", "PP", "ParPat", "ParallelPatterns"},
					  {"3","pft","PFT","ParFuncThrow","ParallelFuncThrow"},
					  {"4","pm","PM","PatMatch","PatternMatching"},
					  {"5","test","Test"}});
	
	string al = args.findFlag({{"0","t","T","Threaded"},
				    {"1","tf","TF","FlipThreaded"},
				    {"2","tp","TP","PhaseThreaded"}});
	
	int nt = args.getAsInt(al,to_string(thread::hardware_concurrency()));
	if(args.wasFatal()){
		cout << "Missing Args" << endl;
		exit(1);
	}
	Analysis* a;
	switch(stoi(analysis)){
		case 0:
			a = new DistNaive();
			break;
		case 1:
			a = new FuncThrow();
			break;
		case 2:
			a = new ParallelPatterns();
			break;
		case 3:
			a = new ParallelFuncThrow();
			break;
		case 4:
			a = new PatternMatching();
			break;
		case 5:
			a = new Distributed();
	}
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
			break;
	}


	a->start();
	a->run();
	a->stop();
	if(a->state->debug){
		cout << "Total program running time: " << a->state->time << endl;
	}

	return 0;
}

