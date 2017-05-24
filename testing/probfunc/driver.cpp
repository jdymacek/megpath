//ProbFunc Testing File
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/24/2017

#include <iostream>
#include <cstdlib>
#include "../../shared/ProbFunc.h"
#include "../../shared/FixedPF.h"
#include "../../shared/HistoPF.h"
#include "../../shared/UniformPF.h"

using namespace std;

void fixedTest(){
	ProbFunc* pf = new FixedPF(0,1,0.5);
	for(int i = 0; i < 1000; ++i){
		vector<Entry> vec = pf->random();
		if(vec[0].val <= 0.495 || vec[0].val >= 0.505){
			cout << vec[0].val << "\n";
		}
	}

}

void histoTest(){
	ProbFunc* pf = new HistoPF(0,1);
	normal_distribution<double> distro(0.5,0.1);
	for(int i = 0; i < 5000; ++i){
		double d = distro(ProbFunc::generator);
		if(d >= 0 && d <= 1){
			pf->addObservation(d);
		}
	}
//	cout << pf->toString() << endl;
	for(int i = 0; i < 5000; ++i){
		vector<Entry> vec = pf->random();
		cout << vec[0].val << "\n";
	}
}

void shiftTest(){

}

void uniformTest(){
	ProbFunc* pf = new UniformPF(0,1);
	vector<double> counts(10,0);
	for(int i = 0; i < 1000; ++i){
		vector<Entry> vec = pf->random();
		counts[(int)vec[0].val*10] +=1;
	}

	cout << "[ ";
	for(int i = 0; i < counts.size(); ++i){
		if(i < counts.size()-1){
			cout << counts[i] << ", ";
		}else{
			cout << counts[i];
		}
	}
	cout << " ]\n";
}

void normalTest(){

}

int main(int argc, char* argv[]){

	ProbFunc::generator.seed(time(0));

	if(argc < 2){
		cout << "Not enough arguments!\nOptions:\n"
		     << "-fixed\n"
		     << "-histo\n"
		     << "-uniform\n"
		     << "-shift\n"
		     << "-normal\n";
		exit(1);
	}

	string type = argv[1];

	if(type == "-fixed"){
		fixedTest();
	}else if(type == "-histo"){
		histoTest();
	}else if(type == "-uniform"){
		uniformTest();
	}else if(type == "-shift"){
		shiftTest();
	}else{
		normalTest();
	}
	return 0;
}
