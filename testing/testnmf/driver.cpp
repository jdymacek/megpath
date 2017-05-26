//connnmf tester
//Matthew Dyer
//Created on 5/26/2017
//Last Modified: 5/26/2017

#include <iostream>
#include <random>
#include <fstream>
#include "../../../Eigen/Core"

using namespace std;
using namespace Eigen;

void writeCSV(string filename, MatrixXd m){
	ofstream outFile;
	outFile.open(filename);

	for(int i = 0; i < m.rows(); ++i){
		for(int j = 0; j < m.cols(); ++j){
			if(j < m.cols()-1){
				outFile << m(i,j) << ",";
			}else{
				outFile << m(i,j);
			}
		}
		outFile << "\n";
	}
	
	outFile.close();
}

int main(){
	MatrixXd patterns(3,5);
	patterns << 0,0.15,0.2,0.15,0, 0,0.1,0.3,0.1,0, 0.1,0.25,0.5,0.25,0.1;

	default_random_engine gen;
	uniform_real_distribution<double> dist(0.0,1.0);
	MatrixXd coefficients(500,3);
	for(int i = 0; i < coefficients.rows(); ++i){
		for(int j = 0; j < coefficients.cols(); ++j){
			coefficients(i,j) = dist(gen);
		}
	}

	MatrixXd mult = coefficients * patterns;

	writeCSV("patterns.csv",patterns);
	writeCSV("coefficients.csv",coefficients);
	writeCSV("multiplied.csv",mult);

	ofstream outfile;
	outfile.open("arguments.txt");
	outfile << "analysis = \"test\"\n";
	outfile << "test_filename = \"../testing/testnmf/multiplied.csv\"\n";
	outfile << "test_origin = [0,0]\n";
	outfile.close();

	return 0;
}
