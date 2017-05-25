//Main file
//Matthew Dyer
//Created on 5/25/2017
//Last modified: 5/25/2017

#include<iostream>
#include"Value.h"
#include"ArgFile.h"
#include"CSVFile.h"

using namespace std;

int main(){
	ArgFile args;
	CSVFile file;
	string filename = "";

	args.load("arguments.txt");

	if(args.isArgument("filename")){
		Value val = args.getArgument("filename");
		filename = val.asString();
	}

	vector<vector<Value> > csv = file.readCSV(filename);
	for(int i = 0; i < csv.size(); ++i){
		int switcher = 0;
		for(int j = 0; j < csv[i].size(); ++j){
			if(switcher == 0){
				cout << csv[i][j].asString() << " ";
				++switcher;
			}else{
				cout << csv[i][j].asInt() << " ";
			}
		}
		cout << "\n";
	}
	
	return 0;
}
