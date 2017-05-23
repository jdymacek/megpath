//Matthew Dyer
//ArgFile header

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
		string toString();
		Value getArgument(string key);
		bool isArgument(string key);

	private:
		map<string, Value> args;
};

#endif
