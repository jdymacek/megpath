//CSVFile.cpp
//Matthew Dyer
//Created on 5/23/2017
//Last Modified: 6/15/2017

#include "CSVFile.h"
vector<vector<Value> > CSVFile::readCSV(string filename, bool tab){
	vector<vector<Value> > rv;
	Value row;
	string line = "";
	
	ifstream inFile;
	inFile.open(filename);

	if(inFile.fail()){
		cout << "CSVFile could not be opened!\n";
		exit(-1);
	}

	while(getline(inFile,line)){
		if(line[0] == '#'){
			continue;
		}
		if(tab == false){
			line = "[" + line;
			line = line + "]";
			row = Value(line);
			rv.push_back(row);
		}else{
			for(int i = 0; i < line.size(); ++i){
				if(line[i] == '\t'){
					line[i] = ',';
				}
			}
			line = "[" + line;
			line = line + "]";
			row = Value(line);
			rv.push_back(row);
		}
	}

	inFile.close();

	return rv;
}
