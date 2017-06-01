//Tester for Stopwatch
//Matthew Dyer
//Created on 5/26/2017
//Last Modified: 6/1/2017

#include "../../shared/Stopwatch.h"
#include <iostream>
#include <cmath>

using namespace std;

int main(){
	Stopwatch watch;
	
	watch.start();
	for(int i = 0; i < 100000000; ++i){
		int num = sqrt(i);
	}
	string end = watch.formatTime(watch.stop());
	cout << end << "\n";

	cout << watch.formatTime(122) << endl;
	
	return 0;
}
