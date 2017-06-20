#ifndef FUNCTHROW__H
#define FUNCTHROW__H

#include "Distributed.h"
#include <mpi.h>
#include <unistd.h>

using namespace std;

class FuncThrow: public Distributed{
	public:
		FuncThrow();	
		virtual double monteCarlo();
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
	protected:
		int bufferSize;
		double* buffer;
		double* recvBuffer;
};


#endif
