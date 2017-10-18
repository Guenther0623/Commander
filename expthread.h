#ifndef EXPTHREAD_H
#define EXPTHREAD_H

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

#include <QMainWindow>

#include <iostream>

using namespace std;

class Arduino;

class expThread : public QObject
{
	Q_OBJECT

private:

	Arduino* ard;

	string _disabled1;
	string _disabled2;
	bool isDis;

	void countdown(int, bool);

	void door_ChoiceBoth(int);
	void door_ChoiceRight(int);
	void door_ChoiceLeft(int);
	void door_DelayIn(int);
	void door_DelayOut(int);

	void dispensePell(int);
	void dispenseRight(int);
	void dispenseLeft(int);

	void led_Left(int);
	void led_Right(int);
	void led_Mid(int);

	void forceDelay(int);

	void enableMute_1(string);
	void enableMute_2(string);
	void disableMutes();

	void clearBuffer();

	void initArray_DNMSforced(int*, int);
	void initArray_DNMSdelays(int, int*, int);
	void shuffleArrays_DNMS(int*, int*, int);
	void swap(int*, int*);

	void shuffleArray(int*, int);

public:
	bool exp_running;

public slots:
	void startDNMS(int, int, int, int, int, int);

	void startRand();
	void startFish();

signals:
	void sendComm(QString, int);

};

#endif