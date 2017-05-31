//file utility driver
//Matthew Dyer
//Created on 5/31/2017
//Last Modified: 5/31/2017

#include<iostream>
#include "../../shared/FileUtil.h"

using namespace std;

int main(){
	cout << "Is '../../shared' a directory?\n";
	cout << FileUtil::isDirectory("../../shared") << endl;

	cout << "Is '../testnmf/' a directory?\n";
	cout << FileUtil::isDirectory("../testnmf/") << endl;

	cout << "Is '../../shared/FileUtil.h' a directory?\n";
	cout << FileUtil::isDirectory("../../shared/FileUtil.h") << endl;

	cout << "Making the directory 'testDir'.\n";
	FileUtil::mkDirectory("testDir");

	cout << "Making the path: 'testDir/this/path/out/please'.\n";
	FileUtil::mkPath("testDir/this/path/out/please");
	cout << endl;

	cout << "Making a unique file for 'testDir/file.txt'.\n";
	FileUtil::uniqueFile("testDir/file.txt");

	cout << "Making a unique file for 'testDir/file.txt' again.\n";
	FileUtil::uniqueFile("testDir/file.txt");

	return 0;
}
