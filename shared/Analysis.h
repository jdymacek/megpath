#ifndef ANALYSIS__H
#define ANALYSIS__H

#include "State.h"

using namespace std;

class Analysis{
	public:
		Analysis();
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
};

#endif
