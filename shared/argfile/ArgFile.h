//Matthew Dyer
//ArgFile header

struct Arg{
	string type;
	string str;
	double dbl;
	int i;
	vector<string> vec_str;
	vector<int> vec_i;
	vector<double> vec_dbl;
};

#ifndef ARGFILE_H
#define ARGFILE_H

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class ArgFile{
	public:
		void load(string filename);
		string toString();
		(void*) getArgumentAs(string key, string type);

	private:
		map<string, Arg> values;
};

#endif
