#ifndef DISTRIBUTEDGSEA__H
#define DISTRIBUTEDGSEA__H

#include <mpi.h>
#include <unistd.h>
#include <map>
#include <string>
#include "GSEA.h"

using namespace std;

class DistributedGSEA: public GSEA{
	public:
		DistributedGSEA();
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
	protected:
        int rank;
        int size;
        string hostname;
};

#endif
