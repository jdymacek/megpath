#ifndef STANDARD__H
#define STANDARD__H

#include <iostream>
#include "Analysis.h"

using namespace std;
class Standard: public Analysis, public Observer{
	public:
		Standard();
		virtual void monteCallback(double error);
		virtual void annealCallback(double error);
		virtual void run();
		virtual void stop();
};

#endif
