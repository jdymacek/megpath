//Triangle tester
//Julian Dymacek
//Matthew Dyer
//Created On 6/15/2017
//Last Modified: 6/15/2017

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "../../shared/UniformPF.h"

using namespace std;

UniformPF* uniform;

string triangle(double ay, double by, double cy){

	double u = uniform->random();
	double v = uniform->random();

	double MIN = min(u,v);
	double MAX = max(u,v);

	double ax = 0.25;
	double bx = 0.5;
	double cx = 0.75;

	double space = 0.25;

	bool flip = false;	

	if(abs(by-ay) > abs(by-cy)){
		double temp = ay;
		ay = cy;
		cy = temp;
		flip = true;
	}	

	cx = ax + space + space * (1 - (abs(cy-ay)/abs(cy-by)));

	double px = ax*MIN + bx*(1-MAX) + cx*(MAX-MIN);
	double py = ay*MIN + by*(1-MAX) + ay*(MAX-MIN);

	if(flip){
		px = bx-(px-bx);
	}

	stringstream ss;
	ss << px << "," << py << endl;
	return ss.str();
}

int main(){
	ProbFunc::generator.seed(time(0));
	uniform = new UniformPF();

	ofstream outFile;
	outFile.open("triangle.csv");

	for(int i =0; i < 1000; ++i){
		outFile << triangle(15,12,5);
	}

	outFile.close();
	return 0;
}
