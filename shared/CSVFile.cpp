//CSVFile.cpp
//Matthew Dyer
//Created on 5/23/2017
//Last Modified: 5/23/2017

#include "CSVFile.h"
vector<vector<Value> > CSVFile::readCSV(string filename, bool tab){
	vector<vector<Value> > rv;
	Value row;
	string line = "";
	
	ifstream inFile;
	inFile.open(filename);

	while(getline(inFile,line)){
		if(tab == false){
			line = "[" + line;
			line = line + "]";
			row = Value(line);
			rv.push_back(row.asVector());
		}else{
			for(int i = 0; i < line.size(); ++i){
				if(line[i] == '\t'){
					line[i] = ',';
				}
			}
			line = "[" + line;
			line = line + "]";
			row = Value(line);
			rv.push_back(row.asVector());
		}
	}

	inFile.close();

	return rv;
}
