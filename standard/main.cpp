//Main file
//Matthew Dyer
//Julian Dymacek
//Created on 5/25/2017
//Last modified: 5/26/2017
#include <sstream>
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
static NMFMatrix patterns;
static NMFMatrix coefficients;
static MatrixXd  expression;
static MatrixXd  newExpression;

void createNMFMatrix(NMFMatrix& rv,int columns,int rows){
	rv.matrix = MatrixXd(columns,rows);
    rv.matrix = MatrixXd::Zero(columns,rows);
	rv.rows = rows;
	rv.columns = columns;
	rv.functions = new ProbFunc**[columns];
	for(int i =0; i < columns; ++i){
        rv.functions[i] = new ProbFunc*[rows];
        for(int j =0; j < rows; ++j){
            rv.functions[i][j] = new HistoPF();
        }
    }	
}

//vector or string?
string errorDistribution(int b){
	vector<int> bins(b,0);
	for(int x = 0; x < expression.rows(); ++x){
		for(int y = 0; y < expression.cols(); ++y){
			double e = abs(expression(x,y)-newExpression(x,y));
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
	newExpression = coefficients.matrix * patterns.matrix;
	double error = 0;
	for(int x = 0; x < expression.rows(); ++x){
		for(int y = 0; y < expression.cols(); ++y){
			error += abs(expression(x,y)-newExpression(x,y));
		}
	}
	return error;
}

void monteCarloMatrix(NMFMatrix& m){
	double oldError = findError();	

	for(int x =0; x < m.columns; ++x){
		for(int y =0; y < m.rows; ++y){
			ProbFunc* function = m.functions[x][y];
			//double olderror = error;

			double r = function->random();
			if(function->size() == 1){
				m.matrix(x,y) = r;				
			}else{
				for(int k=0; k < function->size(); ++k){
					Entry e = function->getEntry(k);
					m.matrix(e.x,e.y) = e.val;
				}
			}			
			double error = findError();
			if(error < oldError){
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
	cout << "Error Histogram: " << errorDistribution(10) << endl;	
	cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
}

//TODO: does not work at all
bool accept(double de, double t){
	return de < 0 ||  exp(-de/t) > uniform->random();
}

void annealStep(NMFMatrix& m,double t){

	double olderror = findError();
	vector<Entry> entries;
	entries.push_back({0,0,0});

	for(int x =0; x < m.columns; x++){
		for(int y =0; y < m.rows; y++){
			ProbFunc* function = m.functions[x][y];
			double r = function->random();  
			if(function->size() == 1){
				entries[0].x = x;
				entries[0].y = y;
				entries[0].val = m.matrix(x,y);
				m.matrix(x,y) = r;
            }else{
				while(entries.size() < function->size()){
					entries.push_back({0,0,0});
				}
                for(int k=0; k < function->size(); ++k){
                    Entry e = function->getEntry(k);
					double old = m.matrix(e.x,e.y);
			        m.matrix(e.x,e.y) = e.val;
					e.val = old;
					entries[k] = e;
                }
            }		
	
			double error = findError();
			if(!accept(error-olderror,t)){
				for(int i =0; i < function->size(); ++i){
					m.matrix(entries[i].x,entries[i].y) = entries[i].val;
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
			if(formerError - error < 0.005 && error < formerError)
                running = false;
            formerError = error;
		}
		ndx++;
		t *= 0.99975;
	}
    cout << "Error Histogram: " << errorDistribution(10) << endl;
    cout << "Total time: " << watch.formatTime(watch.stop()) << endl;
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

	uniform = new UniformPF();
	
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

	expression = MatrixXd(COLUMNS,ROWS);
	expression = MatrixXd::Zero(COLUMNS,ROWS);

	newExpression = MatrixXd(COLUMNS,ROWS);
    newExpression = MatrixXd::Zero(COLUMNS,ROWS);

	for(int i = 0; i < COLUMNS; ++i){
		for(int j = 0; j < ROWS; ++j){
			expression(i,j) = csv[i+origin[1].asInt()][j+origin[0].asInt()].asDouble();
		}
	}

	normalize(expression);
	
	cout << "After Normalizing:\n";
	cout << expression << "\n";

	//should be PATTERNS and COLUMNS
	createNMFMatrix(patterns,COLUMNS,ROWS);

	//should be ROWS and PATTERNS
	createNMFMatrix(coefficients,COLUMNS,ROWS);

	monteCarlo();
	anneal();		


	return 0;
}
