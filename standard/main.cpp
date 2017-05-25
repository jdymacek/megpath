//Main file
//Matthew Dyer
//Created on 5/25/2017
//Last modified: 5/25/2017

#include<iostream>
#include"Value.h"
#include"ArgFile.h"
#include"CSVFile.h"
#include"../../Eigen/Core"

using namespace std;
using namespace Eigen;

static int ROWS;
static int COLUMNS;

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
	ArgFile args;
	CSVFile file;
	string analysis = "";
	string filename = "";
	vector<Value> origin;
	string dir = "";
	vector<Value> controls;
	vector<Value> columns;
	//vector<Value> patterns;
	
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
	
	COLUMNS = csv.size() - origin[1].asInt();
	ROWS = csv[0].size() - origin[0].asInt();

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
	
	return 0;
}
