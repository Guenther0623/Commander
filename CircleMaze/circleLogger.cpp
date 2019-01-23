#include "circleLogger.h"

#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

CircleLogger::CircleLogger(int mode, string ratN)
{
    //----------------Text File Initializers-------------
    ostringstream ossYear, ossMonth, ossDay, ossHour, ossMin, ossSec;

    ratNum = ratN;
    logMode = logTypes(mode);

    QString directory;

    time_t t = time(nullptr);
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

    if(mode == DEBUG){ 
        directory  = "CircleMazeOutputs/Debug/";
        loggerName = directory.toStdString() + "DebugFiles " + "_Rat" + ratN + stampDate + "_" + stampTime + ".txt";
    }
    else if(mode == TR1){ 
        directory  = "CircleMazeOutputs/TrainingP1/";
        loggerName = directory.toStdString() + "CircleMazeTr1 Rat_" + ratN + stampDate + "_" + stampTime + ".txt";
    }
    else if(mode == TR2){ 
        directory  = "CircleMazeOutputs/TrainingP2/";
        loggerName = directory.toStdString() + "CircleMazeTr2 Rat_" + ratN + stampDate + "_" + stampTime + ".txt";
    }
    else if(mode == EXP){ 
        directory  = "CircleMazeOutputs/Experiment/";
        loggerName = directory.toStdString() + "CircleMazeExp Rat_" + ratN + stampDate + "_" + stampTime + ".txt";
    }   

    QDir dir(directory);

    if(!dir.exists()){
        QDir().mkdir(directory);
    }

    cout << loggerName << endl;

    ofstream temp(loggerName.c_str());
    textFile.open(loggerName.c_str());
}

CircleLogger::~CircleLogger()
{

}

void CircleLogger::log(string comm, int func)
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

void CircleLogger::updateTime()
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
 * If it is Circle, then...
 * 		param1 = start zone
 * 		param2 = total zones
 * 		param3 = total trials
 *
 */

void CircleLogger::printTitle(int param1, int param2, int param3)
{
    ostringstream ossParam1, ossParam2, ossParam3;

    ossParam1 << param1;
    ossParam2 << param2;
    ossParam3 << param3;

    if(logMode == DEBUG){
        log("|         Commander Debug File         |", 0);
        log("|             Version 1.0              |", 0);
        log("|                                      |", 0);
        log("|               " + stampDate + "               |", 0);
        log("****************************************\n", 0);
        log("****************************************\n\n", 0);
        //log("Rat Identification:  " + ratNum, 0);
    }

    if(logMode == TR1){
        log("|           Circle Maze  File          |", 0);
        log("|              Training 1              |", 0);
        log("|                                      |", 0);
        log("|               " + stampDate + "               |", 0);
        log("****************************************\n\n", 0);
        log("**Rat Identification:  " + ratNum, 0);
        log("**Start Zone:  " + ossParam1.str(), 0);
        log("**Total Trials:  " + ossParam3.str(), 0);
        log("****************************************\n\n", 0);


        //log("Rat Identification:  " + ratNum, 0);
    }

    else if(logMode == EXP){
        log("|            Circle Maze Log           |", 0);
        log("|              Experiment              |", 0);
        log("|                                      |", 0);
        log("|               " + stampDate + "               |", 0);
        log("****************************************\n\n", 0);
        log("**Rat Identification:  " + ratNum, 0);
        log("**Start Zone:  " + ossParam1.str(), 0);
        log("**Total Zones:  " + ossParam2.str(), 0);
        log("**Total Trials:  " + ossParam3.str(), 0);
        log("****************************************\n\n", 0);
    }
}