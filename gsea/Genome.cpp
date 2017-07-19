#include "Genome.h"

Genome::Genome(){

}

void Genome::reduce(){
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
}
