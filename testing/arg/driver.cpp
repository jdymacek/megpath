//test args driver
//Matthew Dyer
//Created on 5/23/2017
//Last Modified: 5/23/2017

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
	cout << val.asString() << "\n\n";

	cout << "Vector of ints: \n";
	if(args.isArgument("vecInts")){
		val = args.getArgument("vecInts");
	}
	cout << "[ ";
	for(int i = 0; i < val.asVector().size(); ++i){
		if(i != val.asVector().size()-1){
			cout << val.asVector()[i].asInt() << ",";
		}else{
			cout << val.asVector()[i].asInt();
		}
	}
	cout << " ]\n\n";

	cout << "Vector of doubles: \n";
	if(args.isArgument("vecDoubles")){
		val = args.getArgument("vecDoubles");
	}
	cout << "[ ";
	for(int i = 0; i < val.asVector().size(); ++i){
		if(i != val.asVector().size()-1){
			cout << val.asVector()[i].asDouble() << ",";
		}else{
			cout << val.asVector()[i].asDouble();
		}
	}
	cout << " ]\n\n";

	cout << "Vector of strings: \n";
	if(args.isArgument("vecStrings")){
		val = args.getArgument("vecStrings");
	}
	cout << "[ ";
	for(int i = 0; i < val.asVector().size(); ++i){
		if(i != val.asVector().size()-1){
			cout << val.asVector()[i].asString() << ",";
		}else{
			cout << val.asVector()[i].asString();
		}
	}
	cout << " ]\n\n";

	return 0;
}
