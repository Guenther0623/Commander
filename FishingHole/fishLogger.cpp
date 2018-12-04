#include "fishLogger.h"

#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



using namespace std;

FishLogger::FishLogger(int m, string ratN)
{
	//----------------Text File Initializers-------------
	ostringstream ossYear, ossMonth, ossDay, ossHour, ossMin, ossSec;

	ratNum = ratN;
	mode = m;

	QString directory;

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
	string stampTimeT = hourStamp + "-" + minStamp + "-" + secStamp;
	stampDate = monthStamp + "-" + dayStamp + "-" + ossYear.str();

	if(mode == _MODE_DEBUG){
		directory = "FishingHoleOutputs/Debug/";
		loggerName = directory.toStdString() + "DebugFiles " + stampDate + "_" + stampTimeT + ".txt";
	}
	else if(mode == _MODE_EXPER01){
		directory = "FishingHoleOutputs/Experiment/";
		loggerName = directory.toStdString() + "FishingHoleExp Rat_" + ratN + " Date_" + stampDate + " Time_" + stampTimeT + ".txt";
	}
	else if(mode == _MODE_TRAIN01){
		directory = "FishingHoleOutputs/TrainingP1/";
		loggerName = directory.toStdString() + "FishingHoleTr1 Rat_" + ratN + " Date_" + stampDate + " Time_" + stampTimeT + ".txt";
	}
	else if(mode == _MODE_TRAIN02){
		directory = "FishingHoleOutputs/TrainingP2/";
		loggerName = directory.toStdString() + "FishingHoleTr2 Rat_" + ratN + " Date_" + stampDate + " Time_" + stampTimeT + ".txt";
	}
	else if(mode == _MODE_TRAIN03){
		directory = "FishingHoleOutputs/TrainingP3";
		loggerName = directory.toStdString() + "FishingHoleTr3 Rat_" + ratN + " Date_" + stampDate + " Time_" + stampTimeT + ".txt";
	}

	QDir dir(directory);

	if(!dir.exists()){
		QDir().mkdir(directory);
	}

	cout << loggerName << endl;

	ofstream temp(loggerName.c_str());
	textFile.open(loggerName.c_str());
}

FishLogger::~FishLogger()
{

}

void FishLogger::log(string comm, int func)
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

void FishLogger::updateTime()
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
 *
 * If it is Fishing Hole, then...
 * 		param1 = # of trials
 *		param2 = ITI time
 *		param3 = Probability Mode
 * 		all others are DONT CARES
 */

void FishLogger::printTitle(int param1, int param2, int param3)
{
	ostringstream ossParam1, ossParam2, ossParam3;

	ossParam1 << param1;
	ossParam2 << param2;
	ossParam3 << param3;



	if(mode == _MODE_DEBUG){
		log("|         Commander Debug File         |", 0);
		log("|             Version 1.0              |", 0);
		log("|                                      |", 0);
		log("|               " + stampDate + "               |", 0);
		log("****************************************\n", 0);
	}

	else if(mode == _MODE_EXPER01){
		log("|          Fishing Hole Op Box         |", 0);
		log("|              Experiment              |", 0);
		log("|                                      |", 0);
		log("|               " + stampDate + "               |", 0);
		log("****************************************\n\n", 0);
		log("**Rat Identification:  " + ratNum, 0);
		if (param3 == _NP_ALL)
			log("**Probability Mode: Non-Predictive", 0);
		else if (param3 == _PREDICTIVE) 
			log("**Probability Mode: Predictive", 0);
		else if (param3 == _FIFTYFIFTY)
			log("**Probability Mode: Fifty/Fifty", 0);
	}

	else if(mode == _MODE_TRAIN01){
		log("|          Fishing Hole Op Box         |", 0);
		log("|              Training 1              |", 0);
		log("|                                      |", 0);
		log("|               " + stampDate + "               |", 0);
		log("****************************************\n\n", 0);
		log("**Rat Identification:  " + ratNum, 0);
	}

	else if(mode == _MODE_TRAIN02){
		log("|          Fishing Hole Op Box         |", 0);
		log("|              Training 2              |", 0);
		log("|                                      |", 0);
		log("|               " + stampDate + "               |", 0);
		log("****************************************\n\n", 0);
		log("**Rat Identification:  " + ratNum, 0);
	}

	else if(mode == _MODE_TRAIN03){
		log("|          Fishing Hole Op Box         |", 0);
		log("|              Training 3              |", 0);
		log("|                                      |", 0);
		log("|               " + stampDate + "               |", 0);
		log("****************************************\n\n", 0);
		log("**Rat Identification:  " + ratNum, 0);
	}

	log("**Trials: " + ossParam1.str(), 0);
	log("**ITI: " + ossParam2.str() + "seconds", 0);
	log("****************************************\n\n", 0);

}