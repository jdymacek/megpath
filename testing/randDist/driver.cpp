#include <time.h>
#include <iostream>
#include "NMFMatrix.h"

using namespace std;

int main(int argc, char* argv[]){
	if(argc < 5){
		cout << "Not enough args" << endl;
		return 0;
	}

	srand(time(0));
	MatrixXd total(atoi(argv[2]),atoi(argv[1]));
	MatrixXd add = MatrixXd::Constant(atoi(argv[4]),atoi(argv[3]),1);
	int xRange = total.cols()-add.cols()+1;
	int yRange = total.rows()-add.rows()+1;

	for(int i = 0; i < 10000; i++){
		int randX = (rand()%xRange)*(rand()%xRange)/xRange;
		if(rand()%2 == 0){
			randX = xRange-randX-1;
		}
		int randY = (rand()%yRange)*(rand()%yRange)/yRange;
		if(rand()%2 == 0){
			randY = yRange-randY-1;
		}
		MatrixXd sum = total.block(randY,randX,add.rows(),add.cols());              
		total.block(randY,randX,add.rows(),add.cols()) = sum + add;
	}
	cout << total << endl;
	return 0;
}
