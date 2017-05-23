//test args
#include<iostream>
#include "../../shared/Value.h"
#include "../../shared/ArgFile.h"

using namespace std;

int main(){
	ArgFile args;

	args.load("tests.txt");

	args.toString();

	return 0;
}
