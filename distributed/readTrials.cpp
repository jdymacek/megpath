//Program for reading data from all output files after "collectTrials.sh"
//Matthew Dyer

#include <iostream>
#include <fstream>
#include <Value.h>
#include <ArgFile.h>
#include <FileUtil.h>

using namespace std;

void writeTrials(vector<double> errors, vector<string> times, string filename){
	ofstream outFile;
	outFile.open(filename);
	for(int i = 0; i < errors.size(); ++i){
		outFile << errors[i] << "\t" << times[i] << "\n";
	}
	outFile.close();
}

int main(){
	ArgFile args;
	string filename = "output.txt"	

	vector<double> ppErrors, ftErrors, dnErrors;
	vector<string> ppTimes, ftTimes, dnTimes;

	string prog = "";
	string time = "";
	double error = 0;
	Value val;
	int i = 1;
	while(true){
		if(FileUtil::isFile(filename){
			args.load(filename); //load the file

			//grab the information needed
			if(args.isArgument("Program")){
				val = args.getArgument("Program");
				prog = val.asString();
			}
			if(args.isArgument("Total_running_time")){
				val = args.getArgument("Total_running_time");
				time = val.asString();
			}
			if(args.isArgument("Total_error")){
				val = args.getArgument("Total_error");
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
			filename = filename.substr(0,filename.size()-4) + "_" + ss.str() + ".txt";
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
