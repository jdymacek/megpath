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

int main(){
	ArgFile args;
	CSVFile file;
	string filename = "";

	args.load("arguments.txt");

	if(args.isArgument("filename")){
		Value val = args.getArgument("filename");
		filename = val.asString();
	}

	vector<Value> origin;
	if(args.isArgument("origin")){
		Value val = args.getArgument("origin");
		origin = val.asVector();
	}else{
		Value val;
		origin.push_back(val);
		origin.push_back(val);
	}

	vector<vector<Value> > csv = file.readCSV(filename);
	for(int i = origin[1].asInt(); i < csv.size(); ++i){
		for(int j = origin[0].asInt(); j < csv[i].size(); ++j){
			cout << csv[i][j].asDouble() << " ";
		}
		cout << "\n";
	}
	
	int sizeY = csv.size() - origin[1].asInt();
	int sizeX = csv[0].size() - origin[0].asInt();

	MatrixXd m(sizeY,sizeX);
	m = MatrixXd::Zero(sizeY,sizeX);

	for(int i = 0; i < sizeY; ++i){
		for(int j = 0; j < sizeX; ++j){
			m(i,j) = csv[i+origin[1].asInt()][j+origin[0].asInt()].asDouble();
		}
	}
	
	cout << m << "\n";
	
	return 0;
}
