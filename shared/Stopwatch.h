//Stopwatch.h
//Matthew Dyer
//Created on 5/26/2017
//Last Modified: 5/26/2017

#ifndef STOPWATCH__H
#define STOPWATCH__H

#include <chrono>
#include <string>
using namespace std;

class Stopwatch{
	public:
		void start();
		double stop();
		double lap();
		string formatTime(double num);
	private:
		chrono::time_point<chrono::system_clock> startTime;
		chrono::time_point<chrono::system_clock> stopTime;
        chrono::time_point<chrono::system_clock> lapTime;
};

#endif
