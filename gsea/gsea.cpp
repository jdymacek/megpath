//Gene Set Enrichment Analysis -- Main File
//Matthew Dyer
//Created on 6/27/2017
//Last Modifed 6/29/2017

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
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

void calculateScore(){
	for(int i = 0; i < pathways.size(); ++i){
		double total = 0;

		unordered_set<string>::iterator itSet;
		map<string,Gene>::iterator itMap;
		for(itSet = pathways[i].geneNames.begin(); itSet != pathways[i].geneNames.end();){
			unordered_set<string>::iterator curIt;
			curIt = itSet++;
			itMap = geneMap.find(*curIt);
			if(itMap != geneMap.end()){
				total += itMap->second.coefficients[0];
			}else{
				pathways[i].geneNames.erase(curIt);
			}
		}
		double N = genes.size()	- pathways[i].geneNames.size();
	
		double sum = 0;
		double score = 0;
		itSet = pathways[i].geneNames.begin();
		for(int j = 0; j < genes.size(); ++j){
			itSet = pathways[i].geneNames.find(genes[j].name);
			if(itSet != pathways[i].geneNames.end()){
				sum += genes[j].coefficients[0]/total;
			}else{
				sum -= 1/N;
			}
			if(sum > score){
				score = sum;
			}
		}

		cout << i << ".) \tN=" << N << " \ttotal=" << total << " \tsum=" << sum << " \tscore=" << score << endl;

	}
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

	//output genes
	cout << "Genes: \n";
	for(int i = 0; i < genes.size(); ++i){
		cout << genes[i].id << "\t" << genes[i].name  << "\t";
		for(int j = 0; j < genes[i].coefficients.size(); ++j){
			if(j == genes[i].coefficients.size()-1){
				cout << genes[i].coefficients[j] << "\t";	
			}else{
				cout << genes[i].coefficients[j] << ",";
			}
		}
		cout << genes[i].error << endl;
	}
	cout << endl;

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

	//genes after reduction
	cout << "after reduction: \n";
	for(int i = 0; i < genes.size(); ++i){
		cout << genes[i].id << "\t" << genes[i].name  << "\t";
		for(int j = 0; j < genes[i].coefficients.size(); ++j){
			if(j == genes[i].coefficients.size()-1){
				cout << genes[i].coefficients[j] << "\t";	
			}else{
				cout << genes[i].coefficients[j] << ",";
			}
		}
		cout << genes[i].error << endl;
	}
	cout << endl;

	sort(genes.begin(),genes.end());

	//genes after sort
	cout << "after sort: \n";
	for(int i = 0; i < genes.size(); ++i){
		cout << genes[i].id << "\t" << genes[i].name  << "\t";
		for(int j = 0; j < genes[i].coefficients.size(); ++j){
			if(j == genes[i].coefficients.size()-1){
				cout << genes[i].coefficients[j] << "\t";	
			}else{
				cout << genes[i].coefficients[j] << ",";
			}
		}
		cout << genes[i].error << endl;
	}
	cout << endl;

	//output pathways
	cout << "Pathways: \n";
	for(int i = 0; i < pathways.size(); ++i){
		cout << pathways[i].name << "\t";
		for(auto j : pathways[i].geneNames){
			cout << j << " ";
		}
		cout << endl;
	}
	cout << endl;

	//ouput gene map
	cout << "gene map: \n";
	for(it = geneMap.begin(); it != geneMap.end(); ++it ){
		cout << it->first << "=>" << it->second.coefficients[0] << " ";
	}
	cout << endl << endl;


	calculateScore();


	return 0;
}
