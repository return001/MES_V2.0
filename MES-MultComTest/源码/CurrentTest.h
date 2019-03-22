#pragma once
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

class CurrentTest
{
public:
	CurrentTest();
	~CurrentTest();
	bool RelayChange(int proNum, static char InstrName[], static char RelayInstrName[], int testTimes, int  Delaytime);
	double currentTest(int proNum, static char InstrName[], static char RelayInstrName[], int testTimes, int calibrationTimes, int  Delaytime, double currLowLimit, double curr_HightLimit, double currRange);
	double average(vector<double>::const_iterator begin, vector<double>::const_iterator end, vector<double>::size_type elementsNum);
	bool PowerOn(char InstrName[], double voltage);
	double currentAverage(char InstrName[], int testNum, double currRange);
};
