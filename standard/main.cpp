//Main file
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 6/1/2017

//#define EIGEN_DEFAULT_TO_ROW_MAJOR

#include <sstream>
#include <fstream>
#include <cmath>
#include <vector>
#include <iostream>
#include "NMFMatrix.h"
#include "Stopwatch.h"
#include "Value.h"
#include "ShiftPF.h"
#include "ArgFile.h"
#include "CSVFile.h"
#include "ProbFunc.h"
#include "HistoPF.h"
#include "UniformPF.h"
#include "../../Eigen/Core"
#include "Globals.cpp"

using namespace std;
using namespace Eigen;

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

int main(int argc, char** argv){

	ProbFunc::generator.seed(time(0));

	//declare variables
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

	if(argc < 2){
		cout << "Needs an arguments file!\n";
		return 0;
	}
	string argFile = argv[1];	
	//grab arguments
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

	cout << "Using analysis: " << analysis.substr(0,analysis.size()-1) << "\n";

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

	vector<vector<Value> > csv = file.readCSV(filename);

/*	cout << "From the CSVFile:\n";
	for(int y = origin[1].asInt(); y < csv.size(); ++y){
		for(int x = origin[0].asInt(); x < csv[y].size(); ++x){
			cout << csv[y][x].asDouble() << " ";
		}
		cout << "\n";
	}
	cout << "\n";
*/

	//expression matrix
	ROWS = csv.size() - origin[1].asInt();
	COLUMNS = columns.size();

	expression = MatrixXd(ROWS,COLUMNS);
	expression = MatrixXd::Zero(ROWS,COLUMNS);

	newExpression = MatrixXd(ROWS,COLUMNS);
	newExpression = MatrixXd::Zero(ROWS,COLUMNS);
	
	if(columns.size() != controls.size() && args.isArgument(analysis + "controls")){
		cout << "Columns and controls must be the same size.";
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

	//cout << expression << endl;

	normalize(expression);

	/*	cout << "After Normalizing:\n";
		cout << expression << "\n";
	 */

	//test arg information
	cout << "directory = " << directory << endl;
	cout << "max_runs = " << MAX_RUNS  << endl;
	cout << "controls = [";
	for(int i = 0; i < controls.size(); ++i){
		cout << controls[i].asDouble() << " ";
	}
	cout << "] \n";
	cout << "columns = [";
	for(int i = 0; i < columns.size(); ++i){
		cout << columns[i].asDouble() << " ";
	}
	cout << "] \n";

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

	monteCarlo();
	anneal();		

	patterns.write(analysis + "patterns.csv");
	coefficients.write(analysis + "coefficients.csv");

	ofstream fout;
	fout.open(analysis + "expression.txt");
	fout << coefficients.matrix*patterns.matrix;
	fout.close();
	return 0;
}
