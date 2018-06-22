#ifndef THREEERRORFUNCTION__H
#define THREEERRORFUNCTION__H

#include "ErrorFunction.h"
#include "ThreeState.h"
using namespace std;

class ThreeErrorFunction: public ErrorFunction{
	public:
		ThreeErrorFunction(ThreeState* st);
		virtual double error();
		ThreeState* state;
};

#endif
