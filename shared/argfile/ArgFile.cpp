#include "ArgFile.h"

void ArgFile::load(string filename){
	ifstream inFile;
	inFile.open(filename);

	while(getline(inFile,cin)){
		/* XXX magic XXX */
	}

	infile.close();
}

string ArgFile::toString(){
	return;
}

(void*) ArgFile::getArgumentAs(string key, string type){
	/* XXX more magic XXX */
}
