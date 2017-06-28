//Gene Set Enrichment Analysis -- Main File
//Matthew Dyer
//Created on 6/27/2017
//Last Modifed 6/28/2017

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

vector<Pathway> loadPathways(string filename){
	vector<Pathway> rv;
	ifstream inFile;
	inFile.open(filename);
	while(inFile){		
		Pathway p;	
		p.load(inFile);
		rv.push_back(p);
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
	vector<Pathway> pathways = loadPathways(geneFile);
	
	//get genes
	vector<Gene> genes;
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

	cout << "now sorted by error: \n";
	Gene::sortBy = -1;
	sort(genes.begin(),genes.end());
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

	//pull out the desired pattern
	for(int i = 0; i < genes.size(); ++i){
		string curName = genes[i].name;
		for(int j = i-1; j > 0; --j){
			if(genes[j].name == curName){
				genes.erase(genes.begin()+(i));
			}
		}
	}

	cout << "now sorted by coefficients and reduced: \n";
	Gene::sortBy = 0;
	sort(genes.begin(),genes.end());
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


/*
	map<string,double> patternGenes;
	map<string,double>::iterator it; 
	for(int i = 0; i < genes.size(); ++i){
		patternGenes.insert( pair<string,double>(genes[i].name,genes[i].coefficients[0]) );
	}
	
	cout << "pattern genes: \n";
	for(it = patternGenes.begin(); it != patternGenes.end(); ++it ){
		cout << it->first << "=>" << it->second << " ";
	}
	cout << endl;
*/

	return 0;
}
