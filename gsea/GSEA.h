#ifndef GSEA__H
#define GSEA__H

#include <iostream>
#include "Value.h"
#include "FileUtil.h"
#include "ArgFile.h"
#include "CSVFile.h"
#include <vector>
#include <locale>
#include "Pathway.h"
#include "Genome.h"

using namespace std;

class GSEA {
	public:
		GSEA();
		string uppercase(string str);
		static bool comp(const pair<string,int> &a,const pair<string,int> &b);
		virtual vector<Pathway> loadPathways(string filename);
		virtual void start(string filename);
		virtual void run();
		virtual void stop();
	protected:
		string program;
		string analysis;
		string directory;
		vector<Pathway> pathways;
		Genome genome;

};

#endif
