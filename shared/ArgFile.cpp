#include "ArgFile.h"

ArgFile::ArgFile(){
}

void ArgFile::load(string filename){
	string line = "";
	string key = "";
	string equals = "";
	string value = "";

	ifstream inFile;
	inFile.open(filename);

	while(getline(inFile,line)){
		if(line[0] == '#'){
			continue;
		}else{

			stringstream ss;
			ss << line;
			ss >> key;
			ss >> equals;
			ss >> value;

			Value val = Value(value);
		}

		args.insert(pair<string,Value>(key,val));

	}

	infile.close();
}

string ArgFile::toString(){
	return;
}

Value ArgFile::getArgument(string key){
	Value rv = Value();
	string val = "";

	for(map<string,Value>::iterator i = args.begin(); i != args.end(); ++i){
		if(map->first == key){
			val =  map->second;
			rv = Value(val);
			return rv;
		}
	}

	return rv;
}

bool ArgFile::isArgument(){
	return true;
}
