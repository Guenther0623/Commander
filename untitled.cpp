#include "logger.h"

#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

Logger::Logger(int mode, string ratN)
{
	//----------------Text File Initializers-------------
	ostringstream ossYear, ossMonth, ossDay, ossHour, ossMin, ossSec;

	time_t t = time(0);
	struct tm * now = localtime( & t);

	year = now->tm_year - 100;
	month = now->tm_mon + 1;
	day = now->tm_mday;
	hour = now->tm_hour;
	minu = now->tm_min;
	sec = now->tm_sec;

	ossYear<<year;
	ossMonth<<month;
	ossDay<<day;
	ossHour<<hour;
	ossMin<<minu;
	ossSec<<sec;

	if(sec<10)
		secStamp = "0" + ossSec.str();
	else
		secStamp = ossSec.str();

	if(minu < 10)
		minStamp = "0" + ossMin.str();
	else
		minStamp = ossMin.str();

	if(hour < 10)
		hourStamp = "0" + ossHour.str();
	else
		hourStamp = ossHour.str();

	if(day < 10)
		dayStamp = "0" + ossDay.str();
	else
		dayStamp = ossDay.str();

	if(month < 10)
		monthStamp = "0" + ossMonth.str();
	else
		monthStamp = ossMonth.str();

	stampTime = hourStamp + ":" + minStamp + ":" + secStamp;
	stampDate = monthStamp + "-" + dayStamp + "-" + ossYear.str();

	if(mode == _TEST)
		loggerName = "TextOutputs/Debug Files:" + stampDate + "_" + stampTime + ".txt";

	else if(mode == _DNMS)
		loggerName = "TextOutputs/DNMS Files:" + stampDate + "_" + stampTime + "_Rat" + ratN + ".txt";
}