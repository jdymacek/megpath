//Gene Set Enrichment Analysis -- Main File
//Matthew Dyer
//Created on 6/27/2017
//Last Modifed 7/3/2017

#include "GSEA.h"

GSEA::GSEA(){
}

bool GSEA::comp(const pair<string,int> &a,const pair<string,int> &b){
       return a.second>b.second;
}

vector<Pathway> GSEA::loadPathways(string filename){
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

string GSEA::uppercase(string str){
	for(int i =0; i < str.size(); ++i){
		str[i] = toupper(str[i]);
	}
	return str;
}

void GSEA::start(string filename){
	ArgFile args;
	CSVFile csv;

	//grab arguments
	args.load(filename);

	string coeffFile = "";
	string geneFile = "";

	if(args.isArgument("directory")){
		Value val = args.getArgument("directory");
		directory = val.asString();
	}

	if(args.isArgument("analysis")){
		Value val = args.getArgument("analysis");
		analysis = val.asString();
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
	vector<vector<Value> > coeffCSV = csv.readCSV(coeffFile);

	//read the pathway file
	pathways = loadPathways(geneFile);

	//get genes
	for(int i = 0; i < coeffCSV.size(); ++i){
		Gene g;
		g.id = uppercase(coeffCSV[i][0].toString());
		g.name = uppercase(coeffCSV[i][1].toString());
		g.error = coeffCSV[i][coeffCSV[i].size()-1].asDouble();
		for(int j = 2; j < coeffCSV[i].size()-1; ++j){
			g.coefficients.push_back(coeffCSV[i][j].asDouble());
		}
		genome.genes.push_back(g);
	}

	genome.reduce();

}


void GSEA::run(){
	cout << "need to implement!";
}

void GSEA::stop(){
	string outputDir = directory;
	FileUtil::mkPath(outputDir);
	outputDir = outputDir + "/";

	ofstream fgenes;
	string outputFile = FileUtil::uniqueFile(outputDir + analysis + "-names.csv");
	fgenes.open(outputFile);

	double alpha = 0.05;
	map<string,int> leadingGenes;
	map<string,int>::iterator it;
	double m = pathways.size();
	for(int k =0; k < pathways.size(); ++k){
		if(pathways[k].p <= alpha*(double)k/m){
			double per = (double)(pathways[k].leadingSet.size())/(double)(pathways[k].geneNames.size());
			fgenes << pathways[k].name << ", " << pathways[k].p << ", " << per;
			double error = 0;
			for(int i =0; i < pathways[k].leadingSet.size(); ++i){
				fgenes << ", " << pathways[k].leadingSet[i]; 				
				it = leadingGenes.find(pathways[k].leadingSet[i]);
				if(it != leadingGenes.end()){
					it->second += 1;
				}else{
					leadingGenes.insert(pair<string,int>(pathways[k].leadingSet[i],1));
				}
			}
			fgenes << "\n";
		}
	}
	
	fgenes.close();

	ofstream ftop;
	outputFile = FileUtil::uniqueFile(outputDir + analysis + "-genes.csv");
	ftop.open(outputFile);

	vector<pair<string,int> > v(leadingGenes.begin(),leadingGenes.end());
	sort(v.begin(),v.end(),comp);
	for(int i =0; i < v.size(); ++i){
		if(v[i].second > 9){
			ftop << v[i].first << "\t" << v[i].second << endl;
		}
	}

	ftop.close();
}

