/*
	Distributed main file
	Matthew Dyer
	Created on 5/31/2017
	Last Modified: 6/6/2017
 */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <mpi.h>
#include <string.h>
#include "Globals.h"
#include "ArgFile.h"
#include "CSVFile.h"
#include "Value.h"
#include "Stopwatch.h"
#include "ShiftPF.h"

using namespace std;

double *buf;

/*Run a monte carlo markov chain*/
void monteCarlo(int myRank, char* myHost, int numProcs){
	int tag = 0;
	int flag = 0;
	MPI_Request req;
	MPI_Status status;
	buf = new double[23*patterns.matrix.size()+1];
	double* myBuf = new double[23*patterns.matrix.size()+1];
	Stopwatch watch;
	watch.start();

	//For each spot take a gamble and record outcome
	for(int i =0; i < MAX_RUNS; i++){
		flag = 0;
		monteCarloMatrix(patterns);
		monteCarloMatrix(coefficients);
		if(i % 1000 == 0){
			double error = findError();
			double theirError = 0;

			buf[0] = error;
			memcpy(&buf[1],patterns.matrix.data(),(patterns.matrix.size()*sizeof(double)));

			double *ptr = &buf[patterns.matrix.size()+1];
			for(int i = 0; i < patterns.rows; ++i){
				for(int j = 0; j < patterns.columns; ++j){
					vector<double> vec = patterns.functions[i][j]->getVector();
					memcpy(ptr,vec.data(),vec.size()*sizeof(double));
					ptr += vec.size();
				}
			}

			int rando = (rand()%numProcs-1)+1;
			MPI_Isend(buf,23*patterns.matrix.size()+1,MPI_DOUBLE,rando,tag,MPI_COMM_WORLD,&req);
			MPI_Iprobe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&flag,&status);
			if(flag == 1){
				int source = status.MPI_SOURCE;
				MPI_Recv(myBuf,23*patterns.matrix.size()+1,MPI_DOUBLE,source,tag,MPI_COMM_WORLD,&status);
				if(myBuf[0] < error){
					cout << myHost << ": My error was " << error << ". The better error was " << myBuf[0] << ".\n";
					int rows = patterns.matrix.rows();
					int columns = patterns.matrix.cols();
					Map<MatrixXd> mapper(&myBuf[1],rows,columns);
					patterns.matrix = mapper;

					double *ptr = &myBuf[patterns.matrix.size()+1];
					for(int i = 0; i < patterns.rows; ++i){
						for(int j = 0; j < patterns.columns; ++j){
							vector<double> vec = patterns.functions[i][j]->getVector();
							for(int k = 0; k < vec.size(); ++k){
								vec[k] = *ptr;
								++ptr;
							}
							patterns.functions[i][j]->setVector(vec);
						}
					}

				}
			}
			cout << myHost << ": " << i << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
		}
	}
	cout << myHost << "\tFinal Error: " << findError() << endl;
	cout << myHost << "\tError Histogram: " << errorDistribution(10) << endl;	
	cout << myHost << "\tTotal time: " << watch.formatTime(watch.stop()) << endl;
	delete myBuf;
}

void anneal(int myRank, char* myHost, int numProcs){
	int tag = 0;
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
			cout << myHost << ": " << ndx << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
			if(abs(formerError - error) < 0.005 && error < formerError)
				running = false;
			formerError = error;
		}
		ndx++;
		t *= 0.99975;
	}
	formerError = findError();
	cout << myHost << "\t----------------------------------------------------Final Error: " << findError() << endl;
	cout << myHost << "\tError Histogram: " << errorDistribution(10) << endl;	
	cout << myHost << "\tTotal time: " << watch.formatTime(watch.stop()) << endl;

	MPI_Send(&formerError,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD);
}

int main(int argc, char*argv[]){
	Stopwatch watch;
	watch.start();

	if(argc < 2){
		cout << "Needs an arguments file!\n";
		return 0;
	}

	ProbFunc::generator.seed(time(0));
	srand(time(0));
	string argFile = argv[1];
	int minRank = 0;

	//MPI variables
	int process = 0;
	int rank = 0;
	char hostname[100];
	int tag = 0;
	MPI_Status status;

	int flag = 0;

	//Initialize MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &process);
	gethostname(hostname,99);

	//grab arguments
	int ROWS = 0;
	int COLUMNS = 0;
	int PATTERNS = 0;
	ArgFile args;
	CSVFile file;
	string analysis = "";
	string filename = "";
	vector<Value> origin;
	string directory = "";
	vector<Value> controls;
	vector<Value> columns;
	vector<Value> patternArgs;

	uniform = new UniformPF();

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


	cout << hostname << " got : " << analysis << " " << MAX_RUNS << " " << filename << endl;

	vector<vector<Value> > csv = file.readCSV(filename);

	//expression matrix
	ROWS = csv.size() - origin[1].asInt();
	if(args.isArgument(analysis + "columns")){
		Value val = args.getArgument(analysis + "columns");
		columns = val.asVector();
		COLUMNS = columns.size();
	}else{
		COLUMNS = csv[0].size() - origin[0].asInt();
		for(int i = 0; i < COLUMNS;++i){
			Value newVal(i);
			columns.push_back(newVal);
		}
	}

	double minError = ROWS * COLUMNS;

	expression = MatrixXd(ROWS,COLUMNS);
	expression = MatrixXd::Zero(ROWS,COLUMNS);

	newExpression = MatrixXd(ROWS,COLUMNS);
	newExpression = MatrixXd::Zero(ROWS,COLUMNS);

	if(columns.size() != controls.size() && args.isArgument(analysis + "controls")){
		cout << "Columns and controls must be the same size.\n";
		return 0;
	}

	for(int i = 0; i < ROWS; ++i){
		for(int j = 0; j < COLUMNS; ++j){
			if(columns.size() == controls.size()){
				expression(i,j) = csv[i+origin[1].asInt()][columns[j].asInt()+origin[0].asInt()].asDouble() - csv[i+origin[1].asInt()][controls[j].asInt()+origin[0].asInt()].asDouble();
			}else{
				expression(i,j) = csv[i+origin[1].asInt()][columns[j].asInt()+origin[0].asInt()].asDouble();
			}
		}
	}

	normalize(expression);

	//should be PATTERNS and COLUMNS
	patterns = NMFMatrix(PATTERNS,COLUMNS,&findErrorColumn);


	//should be ROWS and PATTERNS
	coefficients = NMFMatrix(ROWS,PATTERNS,&findErrorRow);

	for(int i = 0; i < patternArgs.size(); ++i){
		vector<Value> intoMatrix;
		string findPattern = patternArgs[i].asString();
		if(args.isArgument(findPattern)){
			Value newVal = args.getArgument(findPattern);
			intoMatrix = newVal.asVector();

			ShiftPF* shared = new ShiftPF();			
			vector<Entry> constraints(intoMatrix.size(),{0,0,0});
			for(int j = 0; j < intoMatrix.size(); ++j){
				patterns.matrix(i,j) = intoMatrix[j].asDouble();
				patterns.functions[i][j] = shared;	
				constraints[j].x = j;
				constraints[j].y = i;
				constraints[j].val = patterns.matrix(i,j);	
			}
			shared->setEntries(constraints);
		}
	}


	if(rank == 0){ //I am the manager
		double err = 0;
		for(int i = 1; i < process; ++i){
			MPI_Recv(&err,1,MPI_DOUBLE,i,tag,MPI_COMM_WORLD,&status);
			cout << status.MPI_SOURCE << " " << err << endl;
			if(err < minError){
				minError = err;
				minRank = i;
			}
		}

		int request = 1;
		MPI_Send(&request,1,MPI_INT,minRank,tag,MPI_COMM_WORLD);

		request = 0;
		for(int i = 1; i < process; ++i){
			if(i != minRank){
				MPI_Send(&request,1,MPI_INT,i,tag,MPI_COMM_WORLD);
			}
		}

		MPI_Recv(patterns.matrix.data(),patterns.matrix.rows()*patterns.matrix.cols(),MPI_DOUBLE,minRank,tag,MPI_COMM_WORLD,&status);
		MPI_Recv(coefficients.matrix.data(),coefficients.matrix.rows()*coefficients.matrix.cols(),MPI_DOUBLE,minRank,tag,MPI_COMM_WORLD,&status);

	}else{ //I am a child
		monteCarlo(rank,hostname,process);
		anneal(rank,hostname,process);
		MPI_Recv(&flag,1,MPI_INT,0,tag,MPI_COMM_WORLD,&status);
		if(flag == 1){
			// I did the best! Send my matrices to the manager
			cout << hostname << " found the smallest error.\n";
			cout << hostname <<" matrix: \n" << patterns.matrix << endl;
			MPI_Send(patterns.matrix.data(),patterns.matrix.rows()*patterns.matrix.cols(),MPI_DOUBLE,0,tag,MPI_COMM_WORLD);
			MPI_Send(coefficients.matrix.data(),coefficients.matrix.rows()*coefficients.matrix.cols(),MPI_DOUBLE,0,tag,MPI_COMM_WORLD);
		}
	}

	//write out the best matrices to files
	if(rank == 0){
		patterns.write(analysis + "patterns.csv");
		coefficients.write(analysis + "coefficients.csv");

		ofstream fout;
		fout.open(analysis + "expression.txt");
		fout << coefficients.matrix*patterns.matrix;
		fout.close();
	}

	MPI_Finalize();

	if(rank == 0){
		cout << "Total program running time: " << watch.formatTime(watch.stop()) << endl;
		cout << "Minimum error: " << minError << endl;
	}
	
	delete buf;

	return 0;
}
