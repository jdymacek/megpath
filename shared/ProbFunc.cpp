//ProbFunc.h
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/25/2017
#include "ProbFunc.h"

default_random_engine ProbFunc::generator;

virtual int ProbFunc::size(){
	return 1;
}

virtual Entry ProbFunc::getEntry(int i){
	Entry ent = {0,0,0}
	return ent;
}
