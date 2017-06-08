#ifndef ERRORFUNCTION__H
#define ERRORFUNCTION__H

#include "State.h"
#include <sstream>
using namespace std;

class ErrorFunction{
	public:
		ErrorFunction(State* st);
		virtual double error();
		virtual double fastError(int y,int x);
		string errorDistribution(int b);
	protected:
		MatrixXd newExpression;	
		State* state;
};

#endif
