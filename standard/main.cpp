//Main file
//Matthew Dyer
//Created on 5/25/2017
//Last modified: 5/25/2017

#include<iostream>
#include"../shared/Value.h"
#include"../shared/ArgFile.h"
#include"../shared/CSVFile.h"

using namespace std;

int main(){
	ArgFile args;
	args.load("arguments.txt");
	
	return 0;
}
