//Gene class for GSEA
//Matthew Dyer
//Created on 6/27/2017
//Last Modifed 6/28/2017

#ifndef GENE__H
#define GENE__H

#include <iostream>
#include <vector>

using namespace std;

class Gene{
	public:
		Gene(); 
		bool operator< (const Gene& g);
		string id;
		string name;
		vector<double> coefficients;
		double error;
};

#endif
