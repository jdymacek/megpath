//Tester for Stopwatch
//Matthew Dyer
//Created on 5/26/2017
//Last Modified: 6/1/2017

#include "../../shared/Stopwatch.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

int main(){
	Stopwatch watch;
	
	watch.start();

	sleep(10);

	string end = watch.formatTime(watch.stop());
	cout << end << "\n";

	cout << "Should say two minutes and two seconds : " << watch.formatTime(122) << endl;
	
	return 0;
}
