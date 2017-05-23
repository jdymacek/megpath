#include "ArgFile.h"

ArgFile::ArgFile(){
}

void ArgFile::load(string filename){
	string line = "";
	string key = "";
	string equals = "";
	string value = "";
	Value val = Value();

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
			val = Value(value);
		}

		args.insert(pair<string,Value>(key,val));
	}

	inFile.close();
}

string ArgFile::toString(){
	string rv = "";
		
	for(map<string,Value>::iterator i = args.begin(); i != args.end(); ++i){
		cout << i->first << " = " << i->second.asString() << "\n";
	}

	return rv;
}

Value ArgFile::getArgument(string key){
	Value rv = Value();
	string val = "";

	for(map<string,Value>::iterator i = args.begin(); i != args.end(); ++i){
		if(i->first == key){
			val = i->second.asString();
			rv = Value(val);
			return rv;
		}
	}

	return rv;
}

bool ArgFile::isArgument(){
	return true;
}
