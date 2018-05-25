#ifndef STANDARD__H
#define STANDARD__H

#include <iostream>
#include "Analysis.h"

using namespace std;
class Standard: public Analysis{
	public:
		Standard();
		virtual void run();
};

#endif
