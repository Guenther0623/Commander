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

	ratNum = ratN;
	logMode = mode;

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

	if(mode == _DEBUG)
		loggerName = "TextOutputs/Debug Files:" + stampDate + "_" + stampTime + ".txt";
	else if(mode == _DNMS)
		loggerName = "TextOutputs/DNMS Files:" + stampDate + "_" + stampTime + "_Rat" + ratN + ".txt";
	else if(mode == _RANDO)
		loggerName = "TextOutputs/Random Files:" + stampDate + "_" + stampTime + "_Rat" + ratN + ".txt";
	else if(mode == _FISH)
		loggerName = "TextOutputs/Fishing Hole Files:" + stampDate + "_" + stampTime + "_Rat" + ratN + ".txt";

	ofstream temp(loggerName.c_str());
	textFile.open(loggerName.c_str());
}

Logger::~Logger()
{

}

void Logger::log(string comm, int func)
{
	string txtOut;

	if(func == 1){
		updateTime();
		txtOut = "[" + stampTime + "]     " + comm;
	}
	else if(func == 0){
		txtOut = comm;
	}

	textFile<<txtOut<<endl;
}

void Logger::updateTime()
{
	ostringstream ossYear;
	ostringstream ossMonth;
	ostringstream ossDay;
	ostringstream ossHour;
	ostringstream ossMin;
	ostringstream ossSec;

	time_t t = time(0);
	struct tm * now = localtime( & t);

	int s = now->tm_sec;
	if(s != sec){
		sec = s;
		ossSec<<sec;
		if(sec < 10)
			secStamp = "0" + ossSec.str();
		else
			secStamp = ossSec.str();
	}
	else{
		return;
	}

	int mi = now->tm_min;
	if(mi != minu){
		minu = mi;
		ossMin<<minu;
		if(minu < 10)
			minStamp = "0" + ossMin.str();
		else
			minStamp = ossMin.str();
	}
	else{
		stampTime = hourStamp + ":" + minStamp + ":" + secStamp;
		return;
	}

	int h = now->tm_hour;
	if(h != hour){
		hour = h;
		ossHour<<hour;
		if(hour < 10)
			hourStamp = "0" + ossHour.str();
		else
			hourStamp = ossHour.str();
	}
	else{
		stampTime = hourStamp + ":" + minStamp + ":" + secStamp;
		return;
	}

	stampTime = hourStamp + ":" + minStamp + ":" + secStamp;
	return;
}

/*
 * Function that prints the first few lines of the experiment log
 * 
 * These are important! They tell us the specifications of the
 * experiment at the beginning of each document.  However, this
 * makes it tricky, as each experiment cares for different values
 *
 * If it is debug, all parameters can be 0
 *
 * If it is DNMS, then... 
 * 		param1 = Reward Amount
 * 		param2 = delay Mode
 * 		param3 = ITE Time
 *
 * If it is Random, then...
 * 		param1 = compStrength
 * 		param2 = rewAmt
 * 		all others are DONT CARES
 *
 * If it is Fishing Hole, then...
 * 		param1 = rewAmt
 * 		all others are DONT CARES
 */

void Logger::printTitle(int param1, int param2, int param3)
{
	ostringstream ossParam1, ossParam2, ossParam3;

	ossParam1 << param1;
	ossParam2 << param2;
	ossParam3 << param3;

	if(logMode == _DEBUG){
		log("|         Commander Debug File         |", 0);
		log("|             Version 1.0              |", 0);
		log("|                                      |", 0);
		log("|               " + stampDate + "               |", 0);
		log("****************************************\n", 0);
		log("****************************************\n\n", 0);
		//log("Rat Identification:  " + ratNum, 0);
	}

	else if(logMode == _DNMS){
		log("|           Spatial DNMS Log           |", 0);
		log("|                                      |", 0);
		log("|                                      |", 0);
		log("|               " + stampDate + "               |", 0);
		log("****************************************\n\n", 0);
		log("**Rat Identification:  " + ratNum, 0);
		log("**Reward Amount:  " + ossParam1.str(), 0);
		if(param2 == _0X2X8X32)
			log("**Delay Values:  0, 2, 8, & 32 Seconds", 0);
		else if(param2 == _0X4X16X32)
			log("**Delay Values:  0, 4, 16, & 32 Seconds", 0);
		log("**ITE:  " + ossParam3.str() + " Seconds", 0);
		log("****************************************\n\n", 0);
	}

	else if(logMode == _RANDO){
		log("|       Random Behavioral Op Box       |", 0);
		log("|                                      |", 0);
		log("|                                      |", 0);
		log("|               " + stampDate + "               |", 0);
		log("****************************************\n\n", 0);
		log("**Rat Identification:  " + ratNum, 0);
		log("**Computer Detection Strength:  " + ossParam1.str(), 0);
		log("**Motor Stpes for Reward:  " + ossParam2.str(), 0);
		log("****************************************\n\n", 0);
	}

	else if(logMode == _FISH){
		log("|         Fishhing Hole Op Box         |", 0);
		log("|                                      |", 0);
		log("|                                      |", 0);
		log("|               " + stampDate + "               |", 0);
		log("****************************************\n\n", 0);
		log("**Rat Identification:  " + ratNum, 0);
		log("**Motor Stpes for Reward:  " + ossParam1.str(), 0);
		log("****************************************\n", 0);
	}
}