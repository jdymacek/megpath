//Stopwatch.cpp
//Matthew Dyer
//Created on 5/26/2017
//Last Modified: 5/26/2017

#include "Stopwatch.h"
#include <sstream>

void Stopwatch::start(){
	startTime = time(0);
	lapTime = time(0);
}

double Stopwatch::stop(){
	stopTime = time(0);
	double diff = difftime(stopTime,startTime);
	return diff;
}

double Stopwatch::lap(){
	time_t newLapTime = time(0);
	double diff = difftime(newLapTime,lapTime);
	lapTime = time(0);
	return diff;
}

string Stopwatch::formatTime(double num){
	double days = 0;
	if(num >= 86400){
		days = num/86400;
		num = num - 86400*days;
	}
	double hours = 0;
	if(num >= 3600){
		hours = num/3600;
		num = num - 3600*hours;
	}
	double minutes = 0; 
	if(num >= 60){
		minutes = num/60;
		num = num - 60*minutes;
	}
	double seconds = num;
	string rv;
	stringstream ss;
	ss << days << "d" << hours << "h" << minutes << "m" << seconds << "s";
	ss >> rv;
	return rv;
}
