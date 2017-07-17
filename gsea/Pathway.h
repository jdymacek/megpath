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
#include <map>
#include <unordered_set>
#include <stack>
#include <random>
#include <chrono>
#include <algorithm>
#include "Gene.h"
#include "Genome.h"

using namespace std;

class Pathway{
	public:
		Pathway();
		bool operator< (const Pathway& other);
		Pathway(const Genome& genome,int n);
		void load(string line);
		string toString();
		void random(const Genome& genome,int n);
		double calculateScore(Genome& genome);
		double simulate(Genome& genome,int n);

		double p;
		string name;
		unordered_set<string> geneNames;
		vector<string> leadingSet;
};

#endif
