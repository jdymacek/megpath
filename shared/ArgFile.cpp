#include "ArgFile.h"

ArgFile::ArgFile(){
}

void ArgFile::load(string filename){
	string line;

	ifstream inFile;
	inFile.open(filename);

	while(getline(inFile,line)){
		if(cin[0] == '#'){
			continue;
		}
	}

	infile.close();
}

string ArgFile::toString(){
	return;
}

Value ArgFile::getArgument(string key){
	return;
}

bool ArgFile::isArgument(){
	return true;
}
