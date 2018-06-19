#include "CmdArgs.h"

CmdArgs::CmdArgs(int argc, char** argv){
	//should have some error checking
	string parameter = "";
	for(int i = argc-1; i >=1 ; --i){
		string arg = string(argv[i]);
		if(isNumber<int>(arg) || isNumber<double>(arg) || arg[0] != '-'){
			parameter = arg;	
		}else{
			arglist[arg.substr(1)] = {arg.substr(1),parameter,false,false};
			parameter = "";
		}
	}

}

string CmdArgs::asString(){
	stringstream sout;
    for(auto p : arglist){
	    Argument a = p.second;
		sout << "\n" << p.first << "\t{" << a.parameter << "," << a.found << "," << a.asked << "}";
    }
	string rv = sout.str();
	if(rv.size() > 0)
		rv = rv.substr(1);
    return rv;
}

string CmdArgs::errors(){
    stringstream sout;
    for(auto p : arglist){
        Argument a = p.second;
		if(!a.found && a.asked)
			sout << "\n" << a.name << "\t{" << a.parameter << "}";
    }
    string rv = sout.str();
    if(rv.size() > 0)
        rv = rv.substr(1);
    return rv;
}

bool CmdArgs::wasFatal(){
	for(auto p : arglist){
		Argument a = p.second;
		if(a.asked && !a.found)
			return true;
	}
	return false;
}


bool CmdArgs::getAsDouble(string arg,double& rv,string val){
	if(arglist.find(arg) != arglist.end()){
		string v = arglist[arg].parameter;
		if(v.size() == 0){
			v = val;
		}
		arglist[arg].found = isNumber<double>(v);
		if(arglist[arg].found){
			rv = stod(v);
		}
		arglist[arg].asked = true;
	}else{
		rv = stod(val);
		arglist[arg] = {arg,"",false,true};
	}
	return arglist[arg].found;
}

double CmdArgs::getAsDouble(string arg, string val){
    double rv = 0;
    getAsDouble(arg,rv,val);
    return rv;
}


bool CmdArgs::getAsInt(string arg,int& rv,string val){
    if(arglist.find(arg) != arglist.end()){
        string v = arglist[arg].parameter;
        if(v.size() == 0){
            v = val;
        }
        arglist[arg].found = isNumber<int>(v);
        if(arglist[arg].found){
            rv = stod(v);
        }
        arglist[arg].asked = true;
    }else{
	rv = stod(val);
        arglist[arg] = {arg,"",false,true};
    }
    return arglist[arg].found;
}

int CmdArgs::getAsInt(string arg, string val){
	int rv = 0;
	getAsInt(arg,rv,val);
	return rv;
}


bool CmdArgs::getAsString(string arg,string& rv,string val){
    if(arglist.find(arg) != arglist.end()){
        rv = arglist[arg].parameter;
        if(rv.size() == 0){
            rv = val;
        }
		arglist[arg].found = true;
        arglist[arg].asked = true;
    }else{
        arglist[arg] = {arg,"",false,true};
    }
    return arglist[arg].found;
}

string CmdArgs::findFlag(string flag){
    if(arglist.find(flag) != arglist.end()){
        arglist[flag].found = true;
        arglist[flag].asked = true;
        return flag;
    }
    arglist[flag] = {flag,"",false,true};
    return "";
}


string CmdArgs::findFlag(vector<string> flags){
    for(string f:flags){
        if(arglist.find(f) != arglist.end()){
            arglist[f].found = true;
            arglist[f].asked = true;
            return f;
        }
    }
    for(string f: flags){
        arglist[f] = {f,"",false,true};
    }


    return "";
}


string CmdArgs::findFlag(vector<vector<string> > flagset){
	for(vector<string> flags: flagset){
		for(string f:flags){
			if(arglist.find(f) != arglist.end()){
				arglist[f].found = true;
				arglist[f].asked = true;
				for(string ff:flags){
					arglist[ff] = arglist[f];
				}
				return flags[0];
			}
		}
	}
    for(vector<string> flags: flagset){
		for(string f: flags){
			arglist[f] = {f,"",false,true};
		}
	}


	return "";
}



