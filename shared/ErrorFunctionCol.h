#ifndef ERRORFUNCTIONCOL__H
#define ERRORFUNCTIONCOL__H
#include "ErrorFunction.h"

using namespace std;

class ErrorFunctionCol: public ErrorFunction{
	public:
		ErrorFunctionCol(State* state);
		double fastError(int y,int x);	
	protected:
		NMFMatrix* patterns;
		NMFMatrix* coefficients;

};

#endif
