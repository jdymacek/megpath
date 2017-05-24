#include "HistoPF.h"

HistoPF::HistoPF(int x,int y){
	Entry e = {x,y,0};
	value.push_back(e);
}

vector<Entry> HistoPF::random(){
	return value;
}

void HistoPF::addObservation(double v){

}

string HistoPF::toString(){
	return "";
}

