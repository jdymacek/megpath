#include "FileUtil.h"
#include "fstream"

bool FileUtil::mkDirectory(string dir){
	if(isDirectory(dir))
		return false;
	if (mkdir(dir.c_str(),0777) != 0){
		return false;
	}
	return true;
}

bool FileUtil::isDirectory(string path){
	struct stat fileStat;
	if(stat(path.c_str(),&fileStat) == 0){
		if(S_ISDIR(fileStat.st_mode)){
			return true;
		}	
	}
	return false;
}

bool FileUtil::mkPath(string path){
	stringstream ss;
	ss.str(path);
	string str;
	string dir = "";
	while(getline(ss,str,'/')){
		dir += str + "/";
		mkDirectory(dir);			
	}
	if(isDirectory(path)){
		return true;
	}
	return false;
}

bool FileUtil::isFile(string file){
	struct stat fileStat;
	if(stat(file.c_str(),&fileStat) == 0){
		if(S_ISREG(fileStat.st_mode)){
			return true;
		}	
	}
	return false;
}

string FileUtil::findMatchingFile(string path, string toSearch, int num){
	int i = 1;
	string rv = path;
	bool notFound = true;
	while(notFound){
		if(isFile(rv)){

			ifstream fin;
			fin.open(rv);
			string tmp = "";
			string toComp = "";
			for(int i = 0; i < num; i++){
				getline(fin, tmp);
				toComp += tmp + "\n";
			}
			//cout << toComp;
			//checks to see if the file's args match ours
			if(toSearch == toComp){
				notFound = true;//this is kinda pointless
				return rv;
			}
			//if they dont check for another file:

			stringstream ss;
			ss.str(path);
			string str = "";
	
			//grab base and extension
			getline(ss,str,'.');
			string base = str;
			getline(ss,str);
			string ext = str;
	
			stringstream append;
			append << i;
			rv = base + "_" + append.str()+ "." + ext;
			++i;
		}else{
			return rv;
		}
	}
	return rv;

}

// use this function to find unique files
string FileUtil::uniqueFile(string path){
	int i = 1;
	string rv = path;
	while(isFile(rv)){
		stringstream ss;
		ss.str(path);
		string str = "";

		//grab base and extension
		getline(ss,str,'.');
		string base = str;
		getline(ss,str);
		string ext = str;

		stringstream append;
		append << i;
		rv = base + "_" + append.str()+ "." + ext;
		++i;
	}
	return rv;
}
