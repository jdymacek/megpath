/*
	Globals.cpp
		Matthew Dyer
			Created on 6/1/2017
			Last Modified: 6/1/2017
*/
#include "Globals.h"

void normalize(MatrixXd& m){
	double max = m.maxCoeff();
	double min = m.minCoeff();
	m = m.array() - min;
	m = m/(max-min);
}

string errorDistribution(int b){
	vector<int> bins(b,0);
	newExpression = coefficients.matrix*patterns.matrix;
	for(int y = 0; y < expression.rows(); ++y){
		for(int x = 0; x < expression.cols(); ++x){
			double e = abs(expression(y,x)-newExpression(y,x));
			bins[(int)(e*bins.size())] += 1;
		}
	}
	stringstream ss;
	ss << "[";
	for(int i =0; i < bins.size(); ++i){
		ss << bins[i];
		if(i != bins.size()-1)
			ss << ",";
	}
	ss << "]";
	return ss.str();
}

double findError(){
	//use patterns and coefficients to generate new matrix
	newExpression.noalias() = expression;
	newExpression.noalias() -= (coefficients.matrix*patterns.matrix);
	return (newExpression.cwiseAbs()).sum();
}

double findErrorRow(int y,int x){
	//multiply row from cofficients to pattern matrix
	newExpression.noalias() = expression.row(y);
	newExpression.noalias() -= (coefficients.matrix.row(y) * patterns.matrix);
	return (newExpression.cwiseAbs()).sum();
}

double findErrorColumn(int y,int x){
	newExpression.noalias() = expression.col(x);
	newExpression.noalias() -= (coefficients.matrix * patterns.matrix.col(x));
	return (newExpression.cwiseAbs()).sum();
}

double findError(NMFMatrix& m,int y,int x){
	ProbFunc* prob = m.functions[y][x];
	if(prob->size() > 1){
		return findError();
	}
	return m.errorFunction(y,x);
}

void monteCarloMatrix(NMFMatrix& m){
	for(int y =0; y < m.rows; ++y){
		for(int x =0; x < m.columns; ++x){
			ProbFunc* function = m.functions[y][x];
			double oldError = findError(m,y,x);
			double r = function->random();
			if(function->size() == 1){
				m.matrix(y,x) = r;				
			}else{
				for(int k=0; k < function->size(); ++k){
					Entry e = function->getEntry(k);
					m.matrix(e.y,e.x) = e.val;
				}
			}			
			double error = findError(m,y,x);
			if(error <= oldError){
				function->addObservation(r);
			}
		}
	}
}

bool accept(double de, double t){
	return de < 0 ||  exp(-de/t) > uniform->random();
}

void annealStep(NMFMatrix& m,double t){

	vector<Entry> entries;
	entries.push_back({0,0,0});

	for(int y =0; y < m.rows; y++){
		for(int x =0; x < m.columns; x++){
			double olderror = findError(m,y,x);

			ProbFunc* function = m.functions[y][x];
			double r = function->random();  
			if(function->size() == 1){
				entries[0].x = x;
				entries[0].y = y;
				entries[0].val = m.matrix(y,x);
				m.matrix(y,x) = r;
			}else{
				while(entries.size() < function->size()){
					entries.push_back({0,0,0});
				}
				for(int k=0; k < function->size(); ++k){
					Entry e = function->getEntry(k);
					double old = m.matrix(e.y,e.x);
					m.matrix(e.y,e.x) = e.val;
					e.val = old;
					entries[k] = e;
				}
			}		

			double error = findError(m,y,x);
			if(!accept(error-olderror,t)){
				for(int i =0; i < function->size(); ++i){
					m.matrix(entries[i].y,entries[i].x) = entries[i].val;
				}
			}
		}
	}
}
