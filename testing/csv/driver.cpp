//CSV test file
//Matthew Dyer
//Created on 5/23/2017
//Last Modified: 5/23/2017

#include<iostream>
#include "../../shared/Value.h"
#include "../../shared/CSVFile.h"

using namespace std;

int main(){
	CSVFile file;

	cout << "Integers only: \n";
	vector<vector<Value> > vec = file.readCSV("ints.txt");
	for(int i = 0; i < vec.size(); ++i){
		for(int j = 0; j < vec[i].size(); ++j){
			cout << vec[i][j].asInt() << " ";
		}
		cout << "\n";
	}

	cout << "\nDoubles only: \n";
	vec = file.readCSV("doubles.txt");
	for(int i = 0; i < vec.size(); ++i){
		for(int j = 0; j < vec[i].size(); ++j){
			cout << vec[i][j].asDouble() << " ";
		}
		cout << "\n";
	}

	cout << "\nStrings only: \n";
	vec = file.readCSV("strings.txt");
	for(int i = 0; i < vec.size(); ++i){
		for(int j = 0; j < vec[i].size(); ++j){
			cout << vec[i][j].asString() << " ";
		}
		cout << "\n";
	}

	cout << "\n";

	return 0;
}
