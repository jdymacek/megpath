//Program for reading data from all output files after "collectTrials.sh"
//Matthew Dyer

#include <iostream>
#include <fstream>
#include <vector>
#include "../shared/Value.h"
#include "../shared/ArgFile.h"
#include "../shared/FileUtil.h"

using namespace std;

void writeTrials(vector<double> errors, vector<string> times, string filename){
	ofstream outFile;
	outFile.open(filename);
	for(int i = 0; i < errors.size(); ++i){
		outFile << errors[i] << "," << times[i] << "\n";
	}
	outFile.close();
}

Value getVal(string file, string item){
	string line;
	string key;
	string equals;
	string value;
	Value val = Value();

	ifstream inFile;
	inFile.open(file);
	
	while(getline(inFile,line)){
		stringstream ss;
		ss << line;
		ss >> key;
		if(key == item){
			ss >> equals;
			ss >> value;
			if(item == "Total_running_time" || item == "Program"){
				value = "\"" + value + "\"";
			}
			val = Value(value);
		}
	}
	return val;
}

int main(){
	ArgFile args;
	string filename = "results/output.txt";
	string baseFile = filename;

	vector<double> ppErrors, ftErrors, dnErrors;
	vector<string> ppTimes, ftTimes, dnTimes;

	string prog = "";
	string time = "";
	double error = 0;
	Value val;
	int i = 1;
	while(true){
		if(FileUtil::isFile(filename)){
			args.load(filename); //load the file

			//grab the information needed
			if(args.isArgument("Program")){
				val = getVal(filename,"Program");
				prog = val.asString();
			}
			if(args.isArgument("Total_running_time")){
				val = getVal(filename,"Total_running_time");
				time = val.asString();
			}
			if(args.isArgument("Total_error")){
				val = getVal(filename,"Total_error");
				error = val.asDouble();
			}
		
			//add it into the correct vector
			if(prog == "DistNaive"){
				dnErrors.push_back(error);
				dnTimes.push_back(time);
			}else if(prog == "FuncThrow"){
				ftErrors.push_back(error);
				ftTimes.push_back(time);
			}else if(prog == "ParallelPatterns"){
				ppErrors.push_back(error);
				ppTimes.push_back(time);
			}

			//jump to next file
			stringstream ss;
			ss << i;
			filename = baseFile.substr(0,baseFile.size()-4) + "_" + ss.str() + ".txt";

		}else{
			break;
		}
		++i;
	}

	writeTrials(dnErrors,dnTimes,"DistNaive_trials.csv");
	writeTrials(ftErrors,ftTimes,"FuncThrow_trials.csv");
	writeTrials(ppErrors,ppTimes,"ParallelPatterns_trials.csv");
	
	return 0;
}
