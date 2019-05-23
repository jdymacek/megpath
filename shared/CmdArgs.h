#ifndef CMDARGS__H
#define CMDARGS__H

#include <sstream>
#include <unordered_map>
#include <iostream>
#include <vector>

using namespace std;

struct Argument{
    string name;
    string parameter;
    bool found;
    bool asked;
};

template<typename Numeric>
bool isNumber(const std::string& s)
{
    Numeric n;
	stringstream sin(s);
	if(sin >> n)
		return true;
	return false;
}

class CmdArgs{
	public:
		CmdArgs(int argc,char** argv);
		bool wasFatal();
		string asString();
		string errors();
	    double getAsDouble(string arg, string val = "x");
		bool getAsDouble(string arg,double& rv,string val = "x");
		int	 getAsInt(string arg, string val = "x");
	    bool getAsInt(string arg,int& rv,string val = "x");
        bool getAsString(string arg,string& rv,string val = "");
		string findFlag(string flag);
		string findFlag(vector<string> flags);
		string findFlag(vector<vector<string> > flagset);
	private:
		unordered_map<string,Argument> arglist;

};


#endif
