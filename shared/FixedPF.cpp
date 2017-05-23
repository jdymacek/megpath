#include "FixedPF.h"



FixedPF::FixedPF(int x,int y, double v){
	Entry e = {x,y,v};
	value.push_back(e);
}


vector<Entry> FixedPF::random(){
	return value;
}

string FixedPF::toString(){
	return "";
}

void FixedPF::addObservation(double d){
}
