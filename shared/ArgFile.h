//Matthew Dyer
//ArgFile header

#ifndef ARGFILE_H
#define ARGFILE_H

#include <iostream>
#include <sstream>
#include <vector>
#include "Value.h"

using namespace std;

class ArgFile{
	public:
		ArgFile();
		void load(string filename);
		string toString();
		Value getArgument(string key);
		bool isArgument();

	private:
		map<string, Value> values;
};

#endif
