//Pathway class for GSEA
//Matthew Dyer
//Created on 6/27/2017
//Last Modifed 6/27/2017

#include "Pathway.h"

Pathway::Pathway(){
	p = 0;
	name = "";
}

bool Pathway::operator< (const Pathway& other){
	return p < other.p;
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

Pathway::Pathway(const Genome& genome,int n){
	p = 0;
	random(genome,n);
}

void Pathway::random(const Genome& genome,int n){
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed); //Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<int> dis(0, genome.genes.size()-1);

	while(geneNames.size() < n){
		int r = dis(gen);
		geneNames.insert(genome.genes[r].name);
	}

}

double Pathway::calculateScore(Genome& genome){
    stack<string> leadStack;

    //calculate total and N
    double total = 0;
    unordered_set<string>::iterator itSet;
    map<string,Gene>::iterator itMap;
    for(itSet = geneNames.begin(); itSet != geneNames.end();){
        unordered_set<string>::iterator curIt;
        curIt = itSet++;
        itMap = genome.geneMap.find(*curIt);
        if(itMap != genome.geneMap.end()){
            total += itMap->second.coefficients[0];
        }else{
            geneNames.erase(curIt);
        }
    }
    double N = genome.genes.size() - geneNames.size();

    //calculate sum and score of current pathway
    double sum = 0;
    double score = 0;
    itSet = geneNames.begin();
    for(int j = 0; j < genome.genes.size(); ++j){
        itSet = geneNames.find(genome.genes[j].name);
        if(itSet != geneNames.end()){
            sum += genome.genes[j].coefficients[0]/total;
            leadStack.push(genome.genes[j].name);
        }else{
            sum -= 1/N;
        }
        if(sum > score){
            score = sum;
            while(!leadStack.empty()){
                leadingSet.push_back(leadStack.top());
                leadStack.pop();
            }
        }
    }
    return score;
}

double Pathway::simulate(Genome& genome,int n){
    double pathScore = calculateScore(genome);
    for(int j =0; j < n; ++j){
        Pathway testPath(genome,geneNames.size());
        double randScore = testPath.calculateScore(genome);
		if(randScore > pathScore || abs(randScore-pathScore) < 0.005){
              p += 1;
        }   
    }   
	return p;
}



