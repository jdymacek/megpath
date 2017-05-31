#include "FileUtil.h"

bool FileUtil::mkDirectory(string dir){
	if(isDirectory(dir))
		return false;
	if (mkdir(dir.c_str(),755) != 0){
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
		dir += "/" + str;
		mkDirectory(dir);			
	}
	if(isDirectory(path)){
		return true;
	}
	return false;
}
