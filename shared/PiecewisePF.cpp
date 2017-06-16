//PiecewisePF -- Histogram based probabilty function
//Julian Dymacek
//Matthew Dyer
//Created 6/15/2017
//Modified on: 6/15/2017

#include "PiecewisePF.h"

PiecewisePF::PiecewisePF(){
	SIZE = 41;
	total = SIZE;
	weights = new double[SIZE];
	for(int i =0; i < SIZE; ++i){
		weights[i] = 1;
	}
	uniform = new UniformPF();
}

PiecewisePF::~PiecewisePF(){
	delete weights;
	delete uniform;
}

double PiecewisePF::random(){
	double r = uniform->random();
	int bin =0;
	double running = weights[bin]/total;
	while(r > running && bin < SIZE){
		bin += 1;
		running += weights[bin]/total;
	}
/*	if(bin == 0){
		cout << "zero" << endl;
	}
	if(bin == SIZE-1){
		cout << SIZE-1 << endl;
	}*/

	double space = 1.0/((double)(SIZE-1)); 
	double center = bin*space + space/2;
	
	double px = uniform->random()*space+bin*space;
	return px;

/*	if(bin == 0)
		return 0;
	if(bin == SIZE-1)
		return 1;

	double ay = weights[bin-1];
	double by = weights[bin];
	double cy = weights[bin+1];

    double u = uniform->random();
    double v = uniform->random();

    double MIN = min(u,v);
    double MAX = max(u,v);




    ay = by - abs(ay-by);
    cy = by - abs(cy-by);


    double space = 1.0/((double)(SIZE-1));

    double ax = (bin-1)*space;
    double bx = bin*space;
    double cx = (bin+1)*space;	

    bool flip = false;

    if(abs(by-ay) > abs(by-cy)){
        double temp = ay;
        ay = cy;
        cy = temp;
        flip = true;
    }

	double px = 0;
	if(abs(ay-by) >= 0.00005){
		cx = ax + space + space * (1 - (abs(cy-ay)/abs(cy-by)));
	}
    px = ax*MIN + bx*(1-MAX) + cx*(MAX-MIN);
    //double py = ay*MIN + by*(1-MAX) + ay*(MAX-MIN);

    if(flip){
        px = bx-(px-bx);
    }

	return px;
*/
}
void PiecewisePF::addObservation(double v){
	total += 1;
	double width = 1.0/((double)(SIZE-1));
	int bin = v*(SIZE-1);
	double center = (double)bin/((double)(SIZE-1)) + width/2;
	double alpha = abs(v-center)/width;
	weights[bin] += 1-alpha;
	if(v > center){
		if(bin+1 < SIZE)
			weights[bin+1] += alpha;
	}else if(bin > 0){
		weights[bin-1] += alpha;
	}

/*	double space = 1.0/((double)(SIZE-1));
	int bin = v*(SIZE-1);
	
	double b = (double)bin/((double)(SIZE-1));	
	double alpha = (v-b)/space;
	weights[bin] += 1-alpha;
	if(bin+1 < SIZE)
		weights[bin+1] += alpha;
*/
}

string PiecewisePF::toString(){
	stringstream ss;
	for(int i =0; i < SIZE; ++i){
		ss << weights[i];
		if(i != SIZE-1){
			ss << ",";
		}
	}
	return ss.str();
}

void PiecewisePF::toDoubles(double* buffer){
	memcpy(buffer,weights,SIZE*sizeof(double));	
}

void PiecewisePF::fromDoubles(double* buffer){
	memcpy(weights,buffer,SIZE*sizeof(double));
	total = 0;
	for(int i =0; i < SIZE; ++i){
		total += weights[i];
	}
}

int PiecewisePF::dataSize(){
	return SIZE;
}

