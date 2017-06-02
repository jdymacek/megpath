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

using namespace std;

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
