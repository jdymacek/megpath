//ProbFunc.h
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/25/2017
#include "ProbFunc.h"

mt19937_64 ProbFunc::generator;

int ProbFunc::size(){
	return 1;
}

Entry ProbFunc::getEntry(int i){
	Entry ent = {0,0,0};
	return ent;
}

int ProbFunc::dataSize(){
	return 0;
}

void ProbFunc::toDoubles(double* buffer){

}

void ProbFunc::fromDoubles(double* buffer){

}

vector<double> ProbFunc::getVector(){
	vector<double> vec;
	return vec;
}

void ProbFunc::setVector(vector<double> vec){
}
