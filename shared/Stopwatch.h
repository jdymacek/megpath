//Stopwatch.h
//Matthew Dyer
//Created on 5/26/2017
//Last Modified: 5/26/2017

#ifndef STOPWATCH__H
#define STOPWATCH__H

#include <iostream>
#include <ctime>

using namespace std;

class Stopwatch{
	public:
		void start();
		double stop();
		double lap();
		string formatTime(double num);
	private:
		time_t startTime;
		time_t stopTime;
		time_t laptime;
}

#endif
