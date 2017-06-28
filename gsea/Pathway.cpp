//Pathway class for GSEA
//Matthew Dyer
//Created on 6/27/2017
//Last Modifed 6/27/2017

#include "Pathway.h"

Pathway::Pathway(){
	name = "";
}

void Pathway::load(ifstream& inFile){
	string line = "";
	string geneName = "";
	string junk;

	getline(inFile,line);

	if(line == ""){
		return;
	}

	stringstream ss;
	ss << line;
	ss >> name;
	ss >> junk;

	while(ss >> geneName){
		geneNames.insert(geneName);
	}
}
