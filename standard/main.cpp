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

void normalize(MatrixXd& m ,int x, int y){
	double max = 0;
	double min = 0;
	for(int i = 0; i < y; ++i){
		for(int j = 0; j < x; ++j){
			if(m(i,j) < min)
				min = m(i,j);
			if(m(i,j) > max)
				max = m(i,j);
		}
	}
	double change = 0 - min;
	for(int i = 0; i < y; ++i){
		for(int j = 0; j < x; ++j){
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
	
	int sizeY = csv.size() - origin[1].asInt();
	int sizeX = csv[0].size() - origin[0].asInt();

	MatrixXd m(sizeY,sizeX);
	m = MatrixXd::Zero(sizeY,sizeX);

	for(int i = 0; i < sizeY; ++i){
		for(int j = 0; j < sizeX; ++j){
			m(i,j) = csv[i+origin[1].asInt()][j+origin[0].asInt()].asDouble();
		}
	}

	normalize(m,sizeX,sizeY);
	
	cout << "After Normalizing:\n";
	cout << m << "\n";
	
	return 0;
}
