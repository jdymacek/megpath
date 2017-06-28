//Pathway class for GSEA
//Matthew Dyer
//Created on 6/27/2017
//Last Modifed 6/27/2017

#ifndef PATHWAY__H
#define PATHWAY__H

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_set>

using namespace std;

class Pathway{
	public:
		Pathway();
		void load(string line);

		string name;
		unordered_set<string> geneNames;
};

#endif
