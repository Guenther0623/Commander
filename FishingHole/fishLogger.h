#ifndef FISHLOGGER_H
#define FISHLOGGER_H

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <QDir>

#include "FishingHole/fish_defines.h"

using namespace std;

class FishLogger
{
private:
	string loggerName;
	int mode;

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

public:
	FishLogger(int mode = 0, string ratN = "");
	~FishLogger();

	ofstream textFile;

	void log(string, int);

	void printTitle(int, int, int);
};

#endif