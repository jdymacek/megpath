//Main file
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 7/7/2017

#include "DistNaive.h"
#include "FuncThrow.h"
#include "Stopwatch.h"
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
	
	if(args.wasFatal()){
		cout << "Missing Args" << endl;
		cout << args.errors() << endl;
		exit(1);
	}
	int runs = args.getAsInt("r","1");
	Distributed* a;
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
	int runTime = args.getAsInt("rt", "-1");
	a->load(argFile);
	if(runTime != -1){
		double prevTime;
		int prevRuns;
		Stopwatch watch;
		int running = 1;
		while(running > 0 ){
			watch.start();
			a->start();
			a->run();
			a->stop();
			double time = watch.stop();
			if(time > runTime-0.5 && time < runTime+0.5){
				running = 0;
			}else if (running == 1){
				running = 2;
				prevRuns = a->state->MAX_RUNS;
				prevTime = time;
				a->state->MAX_RUNS = a->broadcastInt(a->state->MAX_RUNS * runTime/time);
			}else{
				double deltaT = prevTime - time;
				int deltaI = prevRuns - a->state->MAX_RUNS;
				prevRuns = a->state->MAX_RUNS;
				prevTime = time;
				a->state->MAX_RUNS = a->broadcastInt((deltaI/deltaT) * runTime);
			}
			cout << "time/runs: " << time << ":" << a->state->MAX_RUNS << endl;
		}
	}
	for(int i = 0; i < runs;i++){
		a->start();
		a->run();
		a->stop();
	}
	delete a;
	return 0;
}

