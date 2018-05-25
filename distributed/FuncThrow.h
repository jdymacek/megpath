#ifndef FUNCTHROW__H
#define FUNCTHROW__H

#include "Distributed.h"
#include <mpi.h>
#include <unistd.h>

using namespace std;

class FuncThrow: virtual public Distributed, public Observer{
	public:
		FuncThrow();	
		virtual void monteCallback(double error);
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
		void finished();
	protected:
		int bufferSize;
		double* buffer;
		double* recvBuffer;
};


#endif
