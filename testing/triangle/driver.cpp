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

	ay = by - abs(ay-by);
	cy = by - abs(cy-by);

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
	
void writeTriangle(string filename, double a, double b, double c){
	ofstream outFile;
	outFile.open(filename);
	for(int i =0; i < 1000; ++i){
		outFile << triangle(a,b,c);
	}
	outFile.close();
}

int main(){
	ProbFunc::generator.seed(time(0));
	uniform = new UniformPF();

	writeTriangle("10.15.5_triangle.csv",10,15,5);
	writeTriangle("15.5.10_triangle.csv",15,5,10);
	writeTriangle("15-12-5_triangle.csv",15,12,5);

	return 0;
}
