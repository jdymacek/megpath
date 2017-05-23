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

	vector<vector<Value> > vec = file.readCSV("test.txt");

	for(int i = 0; i < vec.size(); ++i){
		for(int j = 0; j < vec[i].size(); ++j){
			cout << vec[i][j].toString() << " ";
		}
		cout << "\n";
	}

	return 0;
}
