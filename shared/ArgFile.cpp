//ArgFile.cpp
//Matthew Dyer
//Created on 5/22/2017
//Last Modified: 5/23/2017

#include "ArgFile.h"

ArgFile::ArgFile(){
}

void ArgFile::load(string filename){
	ifstream inFile;
	inFile.open(filename);
	fromStream(inFile);
	inFile.close();
}

void ArgFile::fromString(string str){
	istringstream ss(str);
	fromStream(ss);
}

void ArgFile::fromStream(istream& stream){
	string line = "";
	string key = "";
	string equals = "";
	string value = "";
	Value val = Value();

	while(getline(stream,line)){
		if(line[0] == '#'){
			continue;
		}else{
			/* XXX breaks if the value has a space within it XXX */
			stringstream ss;
			ss << line;
			ss >> key;
			ss >> equals;
			ss >> value;
			val = Value(value);
		}

		args[key] = val;
	}
}

string ArgFile::toString(){
	string rv = "";
	for(map<string,Value>::iterator i = args.begin(); i != args.end(); ++i){
		rv = rv +  i->first + " = " + i->second.toString() + "\n";
	}
	return rv;
}

Value ArgFile::getArgument(string key){
	Value rv = Value();
	if(args.find(key) != args.end()){
		rv = args[key];
	}

	return rv;
}

bool ArgFile::isArgument(string key){
	return args.find(key) != args.end();
}
