//Pathway class for GSEA
//Matthew Dyer
//Created on 6/27/2017
//Last Modifed 6/27/2017

#include "Pathway.h"

Pathway::Pathway(){
	name = "";
}

void Pathway::load(string line){
	string geneName = "";
	string junk;

	stringstream ss;
	ss << line;
	ss >> name;
	ss >> junk;

	while(ss >> geneName){
		geneNames.insert(geneName);
	}
}
string Pathway::toString(){
	stringstream ss;
	unordered_set<string>::iterator it;
	ss << name << " => ";
	ss << "[ ";
	for(it = geneNames.begin(); it != geneNames.end(); ++it){
		ss << *it << " ";
	}
	ss << "]";
	return ss.str();
}
