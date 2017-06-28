//Gene class for GSEA
//Matthew Dyer
//Created on 6/27/2017
//Last Modifed 6/28/2017

#include "Gene.h"

Gene::Gene(){
	id = "";
	name = "";
	error = 0.0;
}

bool Gene::operator<(const Gene& g){
	return error < g.error;
}
