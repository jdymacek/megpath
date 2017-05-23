//test args
#include<iostream>
#include "Value.h"
#include "ArgFile.h"

using namespace std;

int main(){
	ArgFile args;

	args.load("tests.txt");

	args.toString();

	return 0;
}
