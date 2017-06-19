//State.h
//Julian Dymacek
//Created: 6/6/2017
//Modified: 6/6/2017
#ifndef STATE__H
#define STATE__H

#include <cmath>
#include <fstream>
#include "ArgFile.h"
#include "CSVFile.h"
#include "NMFMatrix.h"
#include "../../Eigen/Core"
#include "ShiftPF.h"

using namespace std;
using namespace Eigen;

class State{
	public:
		State();
		int MAX_RUNS;
		NMFMatrix patterns;
		NMFMatrix coefficients;
		MatrixXd expression;
		string directory;
		string analysis;
		string filename;
		bool load(string argFileName);
		void output();
	protected:
		void normalize();


};

#endif
