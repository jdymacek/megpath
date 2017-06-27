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

	//get pathways
	vector<Pathway> pathways;
	for(int i = 0; i < geneCSV.size(); ++i){
		Pathway path;
		string theName = geneCSV[i][0].asString();
		path.name = theName;
		for(int j = 2; j < geneCSV[i].size(); ++j){
			path.geneNames.push_back(geneCSV[i][j].asString());
		}
		pathways.push_back(path);
	}
	
	//get genes
	vector<Gene> genes;
	for(int i = 0; i < ogCSV.size(); ++i){
		Gene g;
		g.name = ogCSV[i][0].asString();
		g.error = coeffCSV[i][coeffCSV[i].size()-1].asDouble();
		for(int j = 0; j < coeffCSV[i].size()-1; ++j){
			g.coefficients.push_back(coeffCSV[i][j].asDouble());
		}
		genes.push_back(g);
	}

	//check genes and pathways
	cout << "Genes: \n";
	for(int i = 0; i < genes.size(); ++i){
		cout << genes[i].name << "\t";
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

	cout << "Pathways: \n";
	for(int i = 0; i < pathways.size(); ++i){
		cout << pathways[i].name << "\t";
		for(int j = 0; j < pathways[i].geneNames.size(); ++j){
			if(j == pathways[i].geneNames.size()-1){
				cout << pathways[i].geneNames[j];	
			}else{
				cout << pathways[i].geneNames[j] << ",";	
			}
		}
		cout << endl;
	}

	return 0;

}
