#include <time.h>
#include <iostream>
#include "NMFMatrix.h"
#include <cmath>

using namespace std;

void shuffleExpression(MatrixXd& tot){
	PermutationMatrix<Dynamic> rPerm(tot.rows());
	rPerm.setIdentity();
	random_shuffle(rPerm.indices().data(),rPerm.indices().data()+rPerm.indices().size());
	PermutationMatrix<Dynamic> cPerm(tot.cols());
	cPerm.setIdentity();
	random_shuffle(cPerm.indices().data(),cPerm.indices().data()+cPerm.indices().size());
	tot = rPerm * tot * cPerm;
	cout << tot << endl << endl;
	tot = rPerm.inverse() * tot * cPerm.inverse();
	cout << tot << endl << endl;	
}

int main(int argc, char* argv[]){
	if(argc < 3){
		cout << "Not enough args" << endl;
		return 0;
	}

	srand(time(0));
	MatrixXd total(atoi(argv[2]),atoi(argv[1]));
	for(int i = 0; i < total.rows(); i++){
		for(int j =0; j < total.cols(); j++){
			total(i,j) = i + j*total.rows();
		}
	}
	cout << total << endl << endl;
	shuffleExpression(total);
/*	MatrixXd add = MatrixXd::Constant(atoi(argv[4]),atoi(argv[3]),1);
	int xRange = total.cols()+add.cols()+1;
	int yRange = total.rows()+add.rows()+1;

	for(int i = 0; i < 10000; i++){
//		int randX = (rand()%xRange)*(rand()%xRange)/xRange;
//		int randY = (rand()%yRange)*(rand()%yRange)/yRange;
		int minX[5];
		int minY[5];
		for(int i = 0; i < 5; i++){
			minX[i] = rand()%(xRange/2);
			minY[i] = rand()%(yRange/2);
		}
		int randX = *min_element(minX,minX+5);
		if(rand()%2 == 0){
			randX = xRange-randX-1;
		}
		int randY = *min_element(minY,minY+5);
		if(rand()%2 == 0){
			randY = yRange-randY-1;
		}

		int randX = rand()%xRange - add.cols();
		if(randX < 0){
			randX = 0;
		}else if(randX > total.cols()-add.cols()){
			randX = total.cols()-add.cols();
		}
		int randY = rand()%yRange - add.rows();
		if(randY < 0){
			randY = 0;
		}else if(randY > total.rows()-add.rows()){
			randY = total.rows()-add.rows();
		}
		MatrixXd sum = total.block(randY,randX,add.rows(),add.cols());              
		total.block(randY,randX,add.rows(),add.cols()) = sum + add;
	}
	cout << total << endl;
*/
	return 0;
}
