#ifndef ERRORFUNCTIONROW__H
#define ERRORFUNCTIONROW__H
#include "ErrorFunction.h"

using namespace std;

class ErrorFunctionRow: public ErrorFunction{
	public:
		ErrorFunctionRow(State* state);
		double fastError(int y,int x);	
	protected:
		NMFMatrix* patterns;
        NMFMatrix* coefficients;

};

#endif
