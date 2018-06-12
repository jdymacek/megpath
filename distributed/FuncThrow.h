#ifndef FUNCTHROW__H
#define FUNCTHROW__H

#include "Distributed.h"
#include "UniformPF.h"
#include <mpi.h>
#include <unistd.h>

using namespace std;

class FuncThrow: virtual public Distributed{
	public:
		FuncThrow();	
		virtual void monteCallback(int iter);
		virtual void start();
		virtual void run();
		virtual void stop();
		void finished();
	protected:
		UniformPF* prob;
		int bufferSize;
		double* buffer;
		double* recvBuffer;
};


#endif
