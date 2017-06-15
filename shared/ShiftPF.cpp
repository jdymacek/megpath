//ShiftPF.cpp -- Shifts a group of varibles
//julian dymacek
//Matthew Dyer
//created: 5/24/2017
//modified: 6/15/2017

#include "ShiftPF.h"

ShiftPF::ShiftPF(){

}

void ShiftPF::setEntries(vector<Entry> vec){
	org = vec;
	current = vec;
}

ShiftPF::ShiftPF(vector<Entry> vec){
	org = vec;
	current = vec;
}


double ShiftPF::random(){
	double r = function.random();
	for(int i =0; i < org.size(); ++i){
		current[i].val = org[i].val + r;
	}
	return r;
}

string ShiftPF::toString(){
	stringstream ss;
	ss << "shiftpf: [";
	for(int i =0; i < org.size(); ++i){
		ss << current[i].val;
		if(i < current.size()-1)
			ss << ",";
	}	
	ss << "]";
	//should print val
	return ss.str();
}

void ShiftPF::addObservation(double d){
	function.addObservation(d);	
}

int ShiftPF::size(){
	return org.size();
}

Entry ShiftPF::getEntry(int i){
	Entry e;
	if(i < 0 || i > org.size())
		return e;
	return current[i];
}

int ShiftPF::dataSize(){
	return function.dataSize() + org.size()*3;
}

void ShiftPF::toDoubles(double* buffer){
	double* fromEntries = new double[3];
	for(int i = 0; i < org.size(); ++i){
		fromEntries[0] = org[i].x;
		fromEntries[1] = org[i].y;
		fromEntries[2] = org[i].val;
		cout << "copying in entries " << i << endl;
		memcpy(buffer,fromEntries,3*sizeof(double));
		buffer += 3;
	}
	delete fromEntries;
	cout << "that loop ended" << endl;

	buffer += org.size();
	function.toDoubles(buffer);
}

void ShiftPF::fromDoubles(double* buffer){
	Entry ent;
	for(int i = 0; i < org.size()*3; i+=3){
		ent.x = buffer[i];
		ent.y = buffer[i+1];
		ent.val = buffer[i+2];
		org.push_back(ent);
		buffer += 3;
	}

	buffer += org.size();
	function.fromDoubles(buffer);
}
