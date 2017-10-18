#ifndef LOGGER_H
#define LOGGER_H

#include "defines.h"

#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

class Logger
{
private:
	string loggerName;

	int sec;
	int minu;
	int hour;
	int day;
	int month;
	int year;

	string secStamp;
	string minStamp;
	string hourStamp;
	string dayStamp;
	string monthStamp;
	string yearStamp;

	string stampDate;
	string stampTime;

	void updateTime();

	string ratNum;
	int logMode;

public:
	Logger(int mode = 0, string ratN = "");
	~Logger();

	ofstream textFile;

	void log(string, int);

	void printTitle(int, int, int);
};

#endif