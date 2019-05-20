//Gene Set Enrichment Analysis -- Main File
//Matthew Dyer
//Created on 6/27/2017
//Last Modifed 7/3/2017

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <chrono>
#include <stack>
#include "Value.h"
#include "ArgFile.h"
#include "CSVFile.h"
#include "Gene.h"
#include "Pathway.h"

using namespace std;

//declaring globals
vector<Pathway> pathways;

Genome genome;

vector<Pathway> loadPathways(string filename){
	vector<Pathway> rv;
	string line = "";
	ifstream inFile;
	inFile.open(filename);
	while(inFile){
		getline(inFile,line);
		if(line != ""){	
			Pathway p;	
			p.load(line);
			rv.push_back(p);
		}
	}
	inFile.close();
	return rv;
}


int main(int argc, char*argv[]){
	ArgFile args;
	CSVFile csv;

	if(argc < 2){
		cout << "Missing argument file!\n";
		exit(-1);
	}

	string argFileName = argv[1];

	//grab arguments
	args.load(argFileName);

	string coeffFile = "";
	string geneFile = "";

	if(args.isArgument("coefficient_file")){
		Value val = args.getArgument("coefficient_file");
		coeffFile = val.asString();
	}

	if(args.isArgument("genome_file")){
		Value val = args.getArgument("genome_file");
		geneFile = val.asString();
	}

	//read all csv files
	vector<vector<Value> > coeffCSV = csv.readCSV(coeffFile);

	//read the pathway file
	pathways = loadPathways(geneFile);

	//get genes
	for(int i = 0; i < coeffCSV.size(); ++i){
		Gene g;
		g.id = coeffCSV[i][0].toString();
		g.name = coeffCSV[i][1].toString();
		g.error = coeffCSV[i][coeffCSV[i].size()-1].asDouble();
		for(int j = 2; j < coeffCSV[i].size()-1; ++j){
			g.coefficients.push_back(coeffCSV[i][j].asDouble());
		}
		genome.genes.push_back(g);
	}

	genome.reduce();

	for(int i =0; i < pathways.size(); ++i){
		double pathScore = pathways[i].calculateScore(genome);
		for(int j =0; j < 1000; ++j){
			Pathway testPath(genome,pathways[i].geneNames.size());
			double randScore = testPath.calculateScore(genome);
            if(randScore > pathScore || abs(randScore-pathScore) < 0.000005){
				pathways[i].p += 1;    
            }
		}
		pathways[i].p /= 1000;
		cout << i+1 << " " << pathways[i].name << " " << pathways[i].p << endl;
	}

	return 0;
}
