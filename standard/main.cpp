//Main file
//Matthew Dyer
//Created on 5/25/2017
//Last modified: 5/25/2017

#include <iostream>
#include "Value.h"
#include "ArgFile.h"
#include "CSVFile.h"
#include "ProbFunc.h"
#include "HistoPF.h"
#include "../../Eigen/Core"

using namespace std;
using namespace Eigen;

static int ROWS;
static int COLUMNS;
static int MAX_RUNS;

struct NMFMatrix{
	MatrixXd matrix;
	ProbFunc*** functions;
	int rows;
	int columns;	

};

void createNMFMatrix(NMFMatrix& rv,int rows,int columns){
	rv.matrix = MatrixXd(COLUMNS,ROWS);
    rv.matrix = MatrixXd::Zero(COLUMNS,ROWS);
	rv.rows = rows;
	rv.columns = columns;
	rv.functions = new ProbFunc**[rows];
	for(int i =0; i < rows; ++i){
        rv.functions[i] = new ProbFunc*[columns];
        for(int j =0; j < columns; ++j){
            rv.functions[i][j] = new HistoPF();
        }
    }	
}


//where should patterns and coefficients be declared
//where should expression be declared?

void monteCarloMatrix(NMFMatrix& m){
	
	for(int i =0; i < m.rows; i++){
		for(int j =0; j < m.columns; j++){
			ProbFunc* function = m.functions[i][j];
			//double olderror = error;

			double r = function->random();
			if(function->size() == 1){
				//eigen is column major -- c++ is row major
				m.matrix[j][i] = r;				
			}else{
				for(int k=0; k < function->size(); ++k){
					Entry e = function->getEntry(k);
					m.matrix[e.x][e.y] = e.val;
				}
			}			

			//multiply
			//newExpression = coefficents*patterns
			//double error = findError(expression,newExpression)
			//if(error < olderror){
			//	function->addObservation(r);
			//}
		}
	}
}

	
	/*Run a monte carlo markov chain*/
void monteCarlo(){
	//TODO: timing
	//long oldTime = System.currentTimeMillis();

	//For each spot take a gamble and record outcome
	for(int i =0; i < MAX_RUNS; i++){
		monteCarloMatrix(patterns);
		monteCarloMatrix(coefficients);
		if(i % 1000 == 0){
			//TODO: timing
			//long newTime = System.currentTimeMillis();
			//System.out.println("** " + i + " Error = " + error + "\tSeconds: " + (newTime-oldTime)/1000);
			//oldTime = newTime;
		}
		//System.out.println("** Max Error: " + this.format(theExpression.maxError()));
		//System.out.println("** Histogram: " + theExpression.errorHistogram(10));		
	}
}


void normalize(MatrixXd& m){
	double max = 0;
	double min = 0;
	for(int i = 0; i < m.rows(); ++i){
		for(int j = 0; j < m.cols(); ++j){
			if(m(i,j) < min)
				min = m(i,j);
			if(m(i,j) > max)
				max = m(i,j);
		}
	}
	double change = 0 - min;
	for(int i = 0; i < m.rows(); ++i){
		for(int j = 0; j < m.cols(); ++j){
			m(i,j) = (m(i,j) + change)/(max-min);
		}
	}
}

int main(){
	//declare variabls
	ArgFile args;
	CSVFile file;
	string analysis = "";
	string filename = "";
	vector<Value> origin;
	string dir = "";
	vector<Value> controls;
	vector<Value> columns;
	//vector<Value> patterns;
	
	//grab arguments
	args.load("arguments.txt");

	if(args.isArgument("analysis")){
		Value val = args.getArgument("analysis");
		analysis = val.asString();
		analysis = analysis + "_";
	}

	cout << "Using analysis: " << analysis.substr(0,analysis.size()-1) << "\n";

	if(args.isArgument(analysis + "filename")){
		Value val = args.getArgument(analysis + "filename");
		filename = val.asString();
	}

	if(args.isArgument(analysis + "origin")){
		Value val = args.getArgument(analysis + "origin");
		origin = val.asVector();
	}else{
		Value val;
		origin.push_back(val);
		origin.push_back(val);
	}

	if(args.isArgument(analysis + "dir")){
		Value val = args.getArgument(analysis + "dir");
		dir = val.asString();
	}

	if(args.isArgument(analysis + "controls")){
		Value val = args.getArgument(analysis + "controls");
		controls = val.asVector();
	}

	if(args.isArgument(analysis + "columns")){
		Value val = args.getArgument(analysis + "column");
		columns = val.asVector();
	}
	
	cout << "From the CSVFile:\n";
	vector<vector<Value> > csv = file.readCSV(filename);
	for(int i = origin[1].asInt(); i < csv.size(); ++i){
		for(int j = origin[0].asInt(); j < csv[i].size(); ++j){
			cout << csv[i][j].asDouble() << " ";
		}
		cout << "\n";
	}
	cout << "\n";
	
	//expression matrix
	COLUMNS = csv.size() - origin[1].asInt();
	ROWS = csv[0].size() - origin[0].asInt();
	//TODO: get from argument
	MAX_RUNS = 40000;

	MatrixXd expression(COLUMNS,ROWS);
	expression = MatrixXd::Zero(COLUMNS,ROWS);

	for(int i = 0; i < COLUMNS; ++i){
		for(int j = 0; j < ROWS; ++j){
			expression(i,j) = csv[i+origin[1].asInt()][j+origin[0].asInt()].asDouble();
		}
	}

	normalize(expression);
	
	cout << "After Normalizing:\n";
	cout << expression << "\n";

	NMFMatrix patterns;
	//should be PATTERNS and COLUMNS
	createNMFMatrix(patterns,ROWS,COLUMNS);

	NMFMatrix coefficients;
	//should be ROWS and PATTERNS
	createNMFMatrix(coefficients,ROWS,COLUMNS);

	return 0;
}
