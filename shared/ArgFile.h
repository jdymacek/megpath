//ArgFile.h
//Matthew Dyer
//Created on 5/22/2017
//Last Modified: 5/23/2017

#ifndef ARGFILE_H
#define ARGFILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "Value.h"

using namespace std;

class ArgFile{
	public:
		ArgFile();
		void load(string filename);
		void fromString(string str);
		void fromStream(istream& stream);
		string toString();
		Value getArgument(string key);
		bool isArgument(string key);

	private:
		map<string, Value> args;
};

#endif
