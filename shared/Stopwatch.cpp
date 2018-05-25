//Stopwatch.cpp
//Matthew Dyer
//Created on 5/26/2017
//Last Modified: 5/25/2018

#include "Stopwatch.h"
#include <sstream>

void Stopwatch::start(){
	startTime = chrono::system_clock::now();	
	lapTime = startTime;
}

double Stopwatch::stop(){
	stopTime = chrono::system_clock::now();
	chrono::duration<double> elapsed = stopTime-startTime;
	double milli = chrono::duration_cast<chrono::milliseconds>(elapsed).count();
	return milli/1000;
}

double Stopwatch::lap(){
	chrono::time_point<chrono::system_clock> newLapTime;
	newLapTime = chrono::system_clock::now();
	chrono::duration<double> elapsed = newLapTime-lapTime;
	lapTime = newLapTime;
	double milli = chrono::duration_cast<chrono::milliseconds>(elapsed).count();
	return milli/1000;
}

string Stopwatch::formatTime(double num){
	double days = 0;
	if(num >= 86400){
		days = (int)(num/86400);
		num = num - 86400*days;
	}
	double hours = 0;
	if(num >= 3600){
		hours = (int)(num/3600);
		num = num - 3600*hours;
	}
	double minutes = 0; 
	if(num >= 60){
		minutes = (int)(num/60);
		num = num - 60*minutes;
	}
	double seconds = num;
	string rv;
	stringstream ss;
	ss << days << "d" << hours << "h" << minutes << "m" << seconds << "s";
	ss >> rv;
	return rv;
}
