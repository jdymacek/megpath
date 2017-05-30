//Main file
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 5/26/2017

//#define EIGEN_DEFAULT_TO_ROW_MAJOR

#include <sstream>
#include <fstream>
#include <cmath>
#include <vector>
#include <iostream>
#include "Stopwatch.h"
#include "Value.h"
#include "ArgFile.h"
#include "CSVFile.h"
#include "ProbFunc.h"
#include "HistoPF.h"
#include "UniformPF.h"
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

static UniformPF* uniform;
NMFMatrix patterns;
NMFMatrix coefficients;
MatrixXd  expression;
MatrixXd  newExpression;

void writeNMFMatrix(NMFMatrix& mat,string filename){
	ofstream fout;
	fout.open(filename);
	for(int y = 0; y < mat.matrix.rows(); ++y){
		for(int x = 0; x < mat.matrix.cols(); ++x){
			fout << mat.matrix(y,x);
			if(x != mat.matrix.cols()-1)
			 fout << ",";
		}
		fout << "\n";
	}
	fout.close();
}


void createNMFMatrix(NMFMatrix& rv,int rows,int columns){
//	rv.matrix = MatrixXd(rows,columns);
    rv.matrix = MatrixXd::Zero(rows,columns);
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

//vector or string?
string errorDistribution(int b){
	vector<int> bins(b,0);
	newExpression = coefficients.matrix*patterns.matrix;
	for(int y = 0; y < expression.rows(); ++y){
		for(int x = 0; x < expression.cols(); ++x){
			double e = abs(expression(y,x)-newExpression(y,x));
			bins[(int)(e*bins.size())] += 1;
		}
	}
	stringstream ss;
	ss << "[";
	for(int i =0; i < bins.size(); ++i){
		ss << bins[i];
		if(i != bins.size()-1)
			ss << ",";
	}
	ss << "]";
	return ss.str();
}

double findError(){
	//use patterns and coefficients to generate new matrix
	newExpression.noalias() = expression;
	newExpression.noalias() -= (coefficients.matrix*patterns.matrix);
	return (newExpression.cwiseAbs()).sum();
}

void monteCarloMatrix(NMFMatrix& m){
	double oldError = findError();	

	for(int y =0; y < m.rows; ++y){
		for(int x =0; x < m.columns; ++x){
			ProbFunc* function = m.functions[y][x];
			//double olderror = error;

			double r = function->random();
			if(function->size() == 1){
				m.matrix(y,x) = r;				
			}else{
				for(int k=0; k < function->size(); ++k){
					Entry e = function->getEntry(k);
					m.matrix(e.y,e.x) = e.val;
				}
			}			
			double error = findError();
			if(error <= oldError){
				function->addObservation(r);
			}
			//changes stick so the error will as well
			oldError = error;
		}
	}
}

	
	/*Run a monte carlo markov chain*/
void monteCarlo(){
	Stopwatch watch;
	watch.start();
	
	//For each spot take a gamble and record outcome
	for(int i =0; i < MAX_RUNS; i++){
		monteCarloMatrix(patterns);
		monteCarloMatrix(coefficients);
		double error = findError();
		if(i % 1000 == 0){
			cout << i << "\t Error = " << error << "\t Time = " << watch.formatTime(watch.lap()) << endl;
		}
	}
	cout << "Final Error: " << findError() << endl;
	cout << "Error Histogram: " << errorDistribution(10) << endl;	
	cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
}

bool accept(double de, double t){
	return de < 0 ||  exp(-de/t) > uniform->random();
}

void annealStep(NMFMatrix& m,double t){

	double olderror = findError();
	vector<Entry> entries;
	entries.push_back({0,0,0});

	for(int y =0; y < m.rows; y++){
		for(int x =0; x < m.columns; x++){
			ProbFunc* function = m.functions[y][x];
			double r = function->random();  
			if(function->size() == 1){
				entries[0].x = x;
				entries[0].y = y;
				entries[0].val = m.matrix(y,x);
				m.matrix(y,x) = r;
            }else{
				while(entries.size() < function->size()){
					entries.push_back({0,0,0});
				}
                for(int k=0; k < function->size(); ++k){
                    Entry e = function->getEntry(k);
					double old = m.matrix(e.y,e.x);
			        m.matrix(e.y,e.x) = e.val;
					e.val = old;
					entries[k] = e;
                }
            }		
	
			double error = findError();
			if(!accept(error-olderror,t)){
				for(int i =0; i < function->size(); ++i){
					m.matrix(entries[i].y,entries[i].x) = entries[i].val;
				}
				newExpression = coefficients.matrix*patterns.matrix;
			}else{
				olderror = error;
			}
		}
	}
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


void normalize(MatrixXd& m){
	double max = m.maxCoeff();
	double min = m.minCoeff();
	m = m.array() - min;
	m = m/(max-min);
}

int main(int argc, char** argv){

    ProbFunc::generator.seed(time(0));

	//declare variabls
	int PATTERNS = 0;
	ArgFile args;
	CSVFile file;
	string analysis = "";
	string filename = "";
	vector<Value> origin;
	string dir = "";
	vector<Value> controls;
	vector<Value> columns;
	vector<Value> patternArgs;

	uniform = new UniformPF();


	if(argc < 2)
		return 0;
	string argFile = argv[1];	
	//grab arguments
	args.load(argFile);


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
	for(int y = origin[1].asInt(); y < csv.size(); ++y){
		for(int x = origin[0].asInt(); x < csv[y].size(); ++x){
			cout << csv[y][x].asDouble() << " ";
		}
		cout << "\n";
	}
	cout << "\n";
	
	//expression matrix
	ROWS = csv.size() - origin[1].asInt();
	COLUMNS = csv[0].size() - origin[0].asInt();
	//TODO: get from argument
	MAX_RUNS = 20000;

	expression = MatrixXd(ROWS,COLUMNS);
	expression = MatrixXd::Zero(ROWS,COLUMNS);

	newExpression = MatrixXd(ROWS,COLUMNS);
    newExpression = MatrixXd::Zero(ROWS,COLUMNS);

	for(int y = 0; y < ROWS; ++y){
		for(int x = 0; x < COLUMNS; ++x){
			expression(y,x) = csv[y+origin[1].asInt()][x+origin[0].asInt()].asDouble();
		}
	}

	//normalize(expression);
	
	cout << "After Normalizing:\n";
	cout << expression << "\n";

	//should be PATTERNS and COLUMNS
	createNMFMatrix(patterns,PATTERNS,COLUMNS);

	//should be ROWS and PATTERNS
	createNMFMatrix(coefficients,ROWS,PATTERNS);

	monteCarlo();
	anneal();		

	writeNMFMatrix(patterns,"patterns.csv");
	writeNMFMatrix(coefficients,"coefficients.csv");

	ofstream fout;
	fout.open("test_expression.txt");
	fout << coefficients.matrix*patterns.matrix;
	fout.close();
	return 0;
}
