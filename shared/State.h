//State.h
//Julian Dymacek
//Matthew Dyer
//Dakota Martin
//Created: 6/6/2017
//Modified: 6/10/2018

#ifndef STATE__H
#define STATE__H

#include <cmath>
#include "ArgFile.h"
#include "CSVFile.h"
#include "NMFMatrix.h"
#include "../../Eigen/Core"
#include "ShiftPF.h"
#include "PNG.h"

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
		string stats;
		string dist;
		bool both;
		bool debug;
		bool constrained;
		bool img;
		bool gray;
		vector<string> patternNames;
		vector<string> ids;
		int printRuns;
		int interruptRuns;
		double annealCutOff;
		double errorAvg;
		double errorCount;
		double start_error;
		double end_error;
		double start_prob;
		double end_prob;
		virtual bool load(string argFileName);
		//vector<vector<int> > splitRanges(int by)
		Range getRange(int rank);
		double calcT();
		double calcAlpha(double t);
		void patternMatch(MatrixXd& other);
		void reset();
		void MXdToPng(MatrixXd mat, int r, int c, bool g, const char* name);

	protected:
		void normalize();
		void normalizeMatrix(MatrixXd& mat);
		vector<vector<Value> > pixlToVal(Image* png, bool& gray);
};

#endif
