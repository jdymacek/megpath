//Main file
//Matthew Dyer
//Created on 5/25/2017
//Last modified: 5/25/2017

#include<iostream>
#include"../shared/Value.h"
#include"../shared/ArgFile.h"
#include"../shared/CSVFile.h"

using namespace std;

int main(){
	ArgFile args;
	CSVFile file;
	string filename = "";

	args.load("arguments.txt");

	if(args.isArguemnt("filename")){
		Value val = args.getArgument("filename");
		filename = val.asString();
	}

	vector<vector<Value > csv = file.readCSV(filename);
	
	return 0;
}
