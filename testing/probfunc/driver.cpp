//ProbFunc Testing File
//Matthew Dyer
//Created on 5/24/2017
//Last Modified: 5/24/2017

#include<iostream>
#include "../../shared/ProbFunc.h"
#include "../../shared/FixedPF.h"

using namespace std;

void fixedTest(){
	ProbFunc* pf = new FixedPF(1,1,0.5);
	for(int i = 0; i < 1000; ++i){
		vector<Entry> vec = pf->random();
		if(vec[0].val <= 0.495 || vec[0].val >= 0.505){
			cout << vec[0].val << "\n";
		}
	}

}

int main(int argc, char* argv[]){

	if(argc < 2){
		cout << "Not enough arguments!\nOptions:\n"
		     << "-fixed\n";
		exit(1);
	}

	string type = argv[1];


	if(type == "-fixed"){
		fixedTest();
	}

	return 0;
}
