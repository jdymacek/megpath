//CSVFile.h
//Matthew Dyer
//Created on 5/23/2017
//Last Modified: 5/23/2017

#ifndef CSVFILE_H
#define CSVFILE_H

#include<iostream>
#include<fstream>
#include<vector>
#include "Value.h"

using namespace std;

class CSVFile{
	public:
		vector<vector<Value> > readCSV(string filename, bool tab = false);
};

#endif
