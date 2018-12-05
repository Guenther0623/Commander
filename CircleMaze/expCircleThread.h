#ifndef EXPCIRCLETHREAD_H
#define EXPCIRCLETHREAD_H

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
#include <string>
#include <iostream>

using namespace std;

class Arduino;

class expCircleThread : public QObject
{
	Q_OBJECT

private:
	Arduino* ard;

	string _disabled1;
	string _disabled2;
	bool isDis;

	bool dispense(int pos);
	bool playSound();
	bool led(int pos, int state);
	bool ledOn(int pos);
	bool ledOff(int pos);
	void clearBuffer();
	
public:
	bool exp_running;

public slots:
	void startCircle_Exp(int, int, int, int, int);

signals:
	void sendComm(QString, int);
	void updateWindow();
	void naturalEnd();

};

#endif
