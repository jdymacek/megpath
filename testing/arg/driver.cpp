//test args
#include<iostream>
#include "../../shared/Value.h"
#include "../../shared/ArgFile.h"

using namespace std;

int main(){
	ArgFile args;

	args.load("tests.txt");

	cout << "Print out the args: \n";
	string print = args.toString();
	cout << print << "\n";

	cout << "Is time an argument?\n";
	cout << args.isArgument("time") << "\n\n";

	cout << "Get the argument for time: \n";
	Value val = args.getArgument("time");
	cout << val.toString() << "\n";

	return 0;
}