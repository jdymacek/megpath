/*
	FileUtil.h
		Julian Dymacek
		Matthew Dyer
			Created on 5/31/2017
			Last Modified: 5/31/2017
*/

#ifndef FILEUTIL__H
#define FILEUTIL__H

#include <iostream>
#include <sstream>
#include <sys/stat.h>
using namespace std;


class FileUtil{
	public:
		static bool mkDirectory(string dir);
		static bool isDirectory(string path);
		static bool mkPath(string path);
		static bool isFile(string file);
		static string uniqueFile(string path);
};
#endif
