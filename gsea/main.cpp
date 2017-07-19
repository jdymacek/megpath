//Gene Set Enrichment Analysis -- Main File
//Matthew Dyer
//Created on 6/27/2017
//Last Modifed 7/3/2017

#include <iostream>
#include "GSEA.h"
#include "DistributedGSEA.h"

using namespace std;

int main(int argc, char*argv[]){

	if(argc < 2){
		cout << "Missing argument file!\n";
		exit(-1);
	}

	GSEA* algo = new DistributedGSEA();
	algo->start(argv[1]);
	algo->run();
	algo->stop();
	return 0;
}
