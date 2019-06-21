#ifndef CENTRALIZED__H
#define CENDRALIZED__H

#include "BlockParallel.h"

class Centralized: public BlockParallel{
public:
	Centralized();
	void start();
	void run();
	void monteCallback(int iter);
	bool annealCallback(int iter);
protected:
	MPI_Comm waitC;
};

#endif
