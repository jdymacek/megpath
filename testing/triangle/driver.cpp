#include <iostream>
#include <algorithm>
#include "../../shared/UniformPF.h"

using namespace std;


UniformPF* uniform;


void triangle(){

	double u = uniform->random();
	double v = uniform->random();

	double MIN = min(u,v);
	double MAX = max(u,v);


	double ax = 0.25;
	double bx = 0.5;
	double cx = 0.75;

	double space = 0.25;

	double ay = 15;
	double by = 10;
	double cy = 5;
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

	cout << px << "," << py << endl;
}

int main(){
	ProbFunc::generator.seed(time(0));
    uniform = new UniformPF();

	for(int i =0; i < 1000; ++i){
		triangle();
	}
	return 0;
}
