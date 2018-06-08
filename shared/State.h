//State.h
//Julian Dymacek
//Matthew Dyer
//Created: 6/6/2017
//Modified: 6/20/2017

#ifndef STATE__H
#define STATE__H

#include <cmath>
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
		string time;
		string stats;
		bool both;
		bool debug;
		bool constrained;
		vector<string> patternNames;
		vector<string> ids;
		int printRuns;
		int interuptRuns;
		double annealCutOff;
		double errorAvg;
		double errorCount;
		vector<vector<int>> splitRanges(int by);
		bool load(string argFileName);
		void patternMatch(MatrixXd& other);

	protected:
		void normalize();
		void normalizeMatrix(MatrixXd& mat);

};

#endif
