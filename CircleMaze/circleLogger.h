#ifndef CIRCLELOGGER_H
#define CIRCLELOGGER_H

#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>

#include "defines.h"

using namespace std;

class CircleLogger
{
private:
    enum logTypes{
        DEBUG = -1,
        EXP = 1,
    }logMode;

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

public:
    CircleLogger(int mode = 0, string ratN = "");
    ~CircleLogger();

    ofstream textFile;

    void log(string, int);

    void printTitle(int, int, int);
};

#endif
