//ThreeState.h
//Dakota Martin
//Created: 6/22/2018

#ifndef THREESTATE__H
#define THREESTATE__H

#include "State.h"
using namespace std;
using namespace Eigen;

class ThreeState: public State{
	public:
		ThreeState();
		bool load(string ArgFileName);
		NMFMatrix weights;	
};
#endif
