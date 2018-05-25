//Standard Analysis functions
//Julian Dymacek
//Dakota Martin
//Created on 5/23/2018
//Last Modified: 5/23/2018

#include "Standard.h"

Standard::Standard(){
	program = "MonteAnneal";
}


void Standard::run(){
	//Could put stop watch in here
	algorithm->setObserver(this);
	algorithm->monteCarlo();
	algorithm->anneal();		
}
