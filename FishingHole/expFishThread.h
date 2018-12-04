#ifndef EXPFISHTHREAD_H
#define EXPFISHTHREAD_H

#define _OPEN 1
#define _CLOSE 0
#define _LEFT_PELL 10
#define _RIGHT_PELL 11
#define _LEFT_LIQ 20
#define _RIGHT_LIQ 21
#define _ON 1
#define _OFF 0
#define _LR_ON 1 
#define _LR_OFF 0
#define _M_ON 1
#define _M_OFF 0

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <QMainWindow>
#include <unistd.h>
#include <vector>

#include "arduino.h"
#include "defines.h"
#include "FishingHole/fish_defines.h"
#include "expFishThread.h"

using namespace std;

class Arduino;

class expFishThread : public QObject
{
	Q_OBJECT

private:
	Arduino* ard;

	string _disabled1;
	string _disabled2;
	bool isDis;

	void createProbsArray(double*, double*, double*, int);
	void shuffleArray(int*, int);
	void shuffleArrayD(double*, int);

	void swap(int*, int*);
	void swapD(double*, double*);
	int countChars(string, char);

	void countdown(int, bool);

	void dispenseRight();
	void dispenseLeft();

	void led_Left(int);
	void led_Right(int);
	void led_Mid(int);
	void led_LeftRight(int);

	void enableMute_1(string);
	void enableMute_2(string);
	void disableMutes();
	void disableMute();

	void clearBuffer();

	void set_Frequency(int);
	void play_Sound(int);

	int scanning();
	int identify();
	
public:
	bool exp_running;

public slots:
	void startFish_Exp(int, int, int, int, int);
	void startFish_Tr1(int, int, int, int);
	void startFish_Tr2(int, int, int, int);
	void startFish_Tr3(int, int, int, int);

signals:
	void sendComm(QString, int);
	void updateWindow();
	void naturalEnd();

};

#endif
