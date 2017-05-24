//UniformPF.cpp
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/24/2017

#include "UniformPF"

UniformPF::UniformPF(int x, int y){
	Entry ent = {x,y,0};
	value.push_back(ent);
}

vector<Entry> UniformPf::random(){
	int intervals = 10;
	int rolls = 1000;

	uniform_real_distribution<double> distribution(0.0,1.0);

	double num;
	for(int i = 0; i < rolls; ++i){
		num = distribution(generator);
		Entry ent = {x,y,num};
		value.push_back(ent);
	}
	
	return value;
}

void UniformPF::addObservation(double d){

}

string UniformPf::toString(){
	return;
}
