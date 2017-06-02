//HybridPF.h
//Matthew Dyer
//Created on 6/2/2017
//Last Modified: 6/2/2017

#ifndef HYBRIDPF__H
#define HYBRIDPF__H

#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include "ProbFunc.h"
#include "UniformPF.h"
#include "NormalPF.h"

using namespace std;

class HybridPF: public ProbFunc{
	public:
		HybridPF();
		double random();
		void addObservation(double obs);	
		string toString();
	private:
		int n;
		NormalPF norm = NormalPF(0.5,0.1);
		UniformPF unif = UniformPF();
};

#endif
