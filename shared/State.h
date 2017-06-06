//State.h
//Julian Dymacek
//Created: 6/6/2017
//Modified: 6/6/2017
#ifndef STATE__H
#define STATE__H

#include "ArgFile.h"
#include "NMFMatrix.h"
#include "../../Eigen/Core"
#include <cmath>

using namespace std;
using namespace Eigen;

class State{
	public:
		int MAX_RUNS;
		NMFMatrix patterns;
		NMFMatrix coefficients;
		NMFMatrix expression;
		string directory;
		string analysis;
		void read(string filename);
	private:
		void normalize();


};

#endif
