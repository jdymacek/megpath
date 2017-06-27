//Gene Set Enrichment Analysis -- Main File
//Matthew Dyer
//Created om 6/27/2017
//Last Modifed 6/27/2017

#include <iostream>
#include <vector>
#include "Value.h"
#include "ArgFile.h"
#include "CSVFile.h"

using namespace std;

struct Gene{
	string name;
	vector<double> coefficients;
	double error;
};

struct Pathway{
	string name;
	vector<string> geneNames;
};

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

	string ogFile = "";
	string coeffFile = "";
	string geneFile = "";

	if(args.isArgument("original_file")){
		Value val = args.getArgument("original_file");
		ogFile = val.asString();
	}

	if(args.isArgument("coefficient_file")){
		Value val = args.getArgument("coefficient_file");
		coeffFile = val.asString();
	}

	if(args.isArgument("genome_file")){
		Value val = args.getArgument("genome_file");
		geneFile = val.asString();
	}

	//read all csv files
	vector<vector<Value> > geneCSV = csv.readCSV(geneFile,true);
	vector<vector<Value> > coeffCSV = csv.readCSV(coeffFile);
	vector<vector<Value> > ogCSV = csv.readCSV(ogFile);

	cout << "After reading csv files\n"; //XXX

	//get pathways
	vector<Pathway> pathways;
	Pathway path;
	for(int i = 0; i < geneCSV.size(); ++i){
		string theName = geneCSV[i][0].asString();
		path.name = theName;
		for(int j = 2; j < geneCSV[i].size(); ++j){
			path.geneNames.push_back(geneCSV[i][j].asString());
		}
		pathways.push_back(path);
	}
	
	cout << "After pathways\n"; //XXX

	//get genes
	vector<Gene> genes;
	Gene g;
	for(int i = 0; i < ogCSV.size(); ++i){
		g.name = pathways[i].name;
		for(int j = 0; j < ogCSV[i].size(); ++j){
			if(j == ogCSV[i].size()){
				g.error = coeffCSV[i][j].asDouble();
			}else{
				g.coefficients.push_back(coeffCSV[i][j].asDouble());
			}
		}
		genes.push_back(g);
	}

	cout << "After genes\n"; //XXX

	//check genes and pathways
	for(int i = 0; i < genes.size(); ++i){
		cout << genes[i].name << "\t";
		for(int j = 0; j < genes[i].coefficients.size(); ++j){
			if(j == genes[i].coefficients.size()){
				cout << genes[i].coefficients[j] << "\t";	
			}else{
				cout << genes[i].coefficients[j] << ",";
			}
		}
		cout << genes[i].error << endl;
	}

	return 0;

}
