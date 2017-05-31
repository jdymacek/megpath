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

	return 0;
}
