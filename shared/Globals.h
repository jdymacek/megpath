/*
	Globals.h
		Matthew Dyer
			Created on 6/1/2017
			Last Modified: 6/1/2017
*/
#include <iostream>
#include <cmath>
#include "../../Eigen/Core"
#include "NMFMatrix.h"
#include "ProbFunc.h"
#include "UniformPF.h"

using namespace std;
using namespace Eigen;

extern int ROWS;
extern int COLUMNS;
extern int MAX_RUNS;

extern UniformPF* uniform;
NMFMatrix patterns;
NMFMatrix coefficients;
MatrixXd  expression;
MatrixXd  newExpression;

void normalize(MatrixXd& m);

string errorDistribution(int b);

double findError();
double findErrorRow(int y,int x);
double findErrorColumn(int y,int x);
double findError(NMFMatrix& m,int y,int x);

void monteCarloMatrix(NMFMatrix& m);

bool accept(double de, double t);

void annealStep(NMFMatrix& m,double t);
