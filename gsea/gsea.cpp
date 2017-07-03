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
#include "Value.h"
#include "ArgFile.h"
#include "CSVFile.h"
#include "Gene.h"
#include "Pathway.h"

using namespace std;

//declaring globals
vector<Gene> genes;
vector<Pathway> pathways;
map<string,Gene> geneMap;

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

double calculateScore(Pathway& path){
	stack<string> leadStack;

	//calculate total and N
	double total = 0;
	unordered_set<string>::iterator itSet;
	map<string,Gene>::iterator itMap;
	for(itSet = path.geneNames.begin(); itSet != path.geneNames.end();){
		unordered_set<string>::iterator curIt;
		curIt = itSet++;
		itMap = geneMap.find(*curIt);
		if(itMap != geneMap.end()){
			total += itMap->second.coefficients[0];
		}else{
			path.geneNames.erase(curIt);
		}
	}
	double N = genes.size()	- path.geneNames.size();

	//calculate sum and score of current pathway
	double sum = 0;
	double score = 0;
	itSet = path.geneNames.begin();
	for(int j = 0; j < genes.size(); ++j){
		itSet = path.geneNames.find(genes[j].name);
		if(itSet != path.geneNames.end()){
			sum += genes[j].coefficients[0]/total;
			leadStack.push(genes[j].name);
		}else{
			sum -= 1/N;
		}
		if(sum > score){
			score = sum;
			while(!leadStack.empty()){
				path.leadingSet.push_back(leadStack.top());
				leadStack.pop();
			}
		}
	}
	return score;
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
		genes.push_back(g);
	}

	//pull out the desired genes into a map
	map<string,Gene>::iterator it; 
	for(int i = 0; i < genes.size(); ++i){
		it = geneMap.find(genes[i].name);
		if(it != geneMap.end()){
			if(genes[i].error < it->second.error){
				it->second = genes[i];
			}
		}else{
			geneMap.insert( pair<string,Gene>(genes[i].name,genes[i]) );
		}
	}

	//refill the vector
	genes.clear();
	for(it = geneMap.begin(); it != geneMap.end(); ++it){
		genes.push_back(it->second);
	}

	sort(genes.begin(),genes.end());

	//for each pathway
	vector<int> scores(pathways.size(),0);
	vector<Gene> shuffledGenes = genes;
	Pathway testPath;
	for(int i = 0; i < pathways.size(); ++i){
		double pathScore = 0;
		pathScore = calculateScore(pathways[i]);

		//compare against randomly generated pathways
		double randScore = 0;
		for(int j = 0; j < 1000; ++j){
			testPath.geneNames.clear();

			unsigned seed = chrono::system_clock::now().time_since_epoch().count();
			shuffle(shuffledGenes.begin(), shuffledGenes.end(),default_random_engine(seed));
			testPath.name = pathways[i].name;
			for(int k = 0; k < pathways[i].geneNames.size(); ++k){
				testPath.geneNames.insert(shuffledGenes[k].name);
			}

			randScore = calculateScore(testPath);
			if(randScore > pathScore || abs(randScore-pathScore) < 0.000005){
				scores[i] = scores[i] + 1;
			}

		}
	}

	for(int i = 0; i < scores.size(); ++i){
		cout << "Score " << i << ": " << scores[i] <<endl;
	}

	return 0;
}
