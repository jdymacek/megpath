#ifndef GENOME__H
#define GENOME__H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "Gene.h"

using namespace std;

class Genome{
    public:
		Genome();
		void reduce();
		vector<Gene> genes;
		map<string,Gene> geneMap;				

};




#endif
