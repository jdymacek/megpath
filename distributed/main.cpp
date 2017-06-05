/*
	Distributed main file
	Matthew Dyer
	Created on 5/31/2017
	Last Modified: 6/1/2017
 */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <mpi.h>
#include "Globals.h"
#include "ArgFile.h"
#include "CSVFile.h"
#include "Value.h"
#include "Stopwatch.h"

using namespace std;

/*Run a monte carlo markov chain*/
void monteCarlo(){
	Stopwatch watch;
	watch.start();

	//For each spot take a gamble and record outcome
	for(int i =0; i < MAX_RUNS; i++){
		monteCarloMatrix(patterns);
		monteCarloMatrix(coefficients);
		if(i % 1000 == 0){
			double error = findError();
			cout << i << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
		}
	}
	cout << "Final Error: " << findError() << endl;
	cout << "Error Histogram: " << errorDistribution(10) << endl;	
	cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
}

void anneal(){
	Stopwatch watch;
	int ndx = 0;
	double t = 0.5;
	watch.start();

	double formerError = 2*expression.rows()*expression.cols();
	bool running = true;
	while(running && ndx < MAX_RUNS){
		annealStep(coefficients,t);
		annealStep(patterns,t);
		if(ndx % 1000 == 0){
			double error = findError();
			cout << ndx << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
			if(abs(formerError - error) < 0.005 && error < formerError)
				running = false;
			formerError = error;
		}
		ndx++;
		t *= 0.99975;
	}
	formerError = findError();
	cout << "Final Error: " << formerError << endl;
	cout << "Error Histogram: " << errorDistribution(10) << endl;
	cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
}

int main(int argc, char*argv[]){

	if(argc < 2){
		cout << "Needs an arguments file!\n";
		return 0;
	}

	string argFile = argv[1];	

	//MPI variables
	int process = 0;
	int rank = 0;
	char hostname[100];
	int tag = 0;
	int source;
	MPI_Status status;

	//Initialize MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &process);
	gethostname(hostname,99);
		
	//grab arguments
	ArgFile args;
	CSVFile file;
	int PATTERNS = 0;
	int MAX_RUNS = 0;
	string analysis = "";
	string filename = "";
	vector<Value> origin;
	string directory = "";
	vector<Value> controls;
	vector<Value> columns;
	vector<Value> patternArgs;
		
	args.load(argFile);

	if(args.isArgument("analysis")){
		Value val = args.getArgument("analysis");
		analysis = val.asString();
		analysis = analysis + "_";
	}

	if(args.isArgument("max_runs")){
		Value val = args.getArgument("max_runs");
		MAX_RUNS = val.asInt();
	}

	if(args.isArgument(analysis + "filename")){
		Value val = args.getArgument(analysis + "filename");
		filename = val.asString();
	}

	if(args.isArgument(analysis + "patterns")){
		Value val = args.getArgument(analysis + "patterns");
		patternArgs = val.asVector();
		PATTERNS = patternArgs.size();
	}

	if(args.isArgument(analysis + "origin")){
		Value val = args.getArgument(analysis + "origin");
		origin = val.asVector();
	}else{
		Value val;
		origin.push_back(val);
		origin.push_back(val);
	}

	if(args.isArgument(analysis + "directory")){
		Value val = args.getArgument(analysis + "directory");
		directory = val.asString();
	}

	if(args.isArgument(analysis + "controls")){
		Value val = args.getArgument(analysis + "controls");
		controls = val.asVector();
	}

	if(args.isArgument(analysis + "columns")){
		Value val = args.getArgument(analysis + "columns");
		columns = val.asVector();
	}

	cout << hostname << " got : " << analysis << " " << MAX_RUNS << " " << filename << endl;

	//MPI_Send(&y,sizeof(int),MPI_INT,0,tag,MPI_COMM_WORLD);
	//MPI_Recv(&y,sizeof(int),MPI_INT,0,tag,MPI_COMM_WORLD,&status);

	MPI_Finalize();
	return 0;
}
