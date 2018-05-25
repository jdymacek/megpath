#ifndef DISTRIBUTED__H
#define DISTRIBUTED__H

#include <mpi.h>
#include <unistd.h>
#include "Analysis.h"

using namespace std;

class Distributed: public Analysis{
	public:
		Distributed();
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
		void sendMatrix(MatrixXd& matrix,int dest);
		void recvMatrix(MatrixXd& matrix,int src);
	protected:
        int rank;
        int size;
        string hostname;
};

#endif
