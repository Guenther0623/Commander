#ifndef DNMS_H
#define DNMS_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QLabel>
#include <QtGui>
#include <QBoxLayout>

#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class DNMS : public QWidget
{
		Q_OBJECT
private:
	//Arduino* ard;	

	void door_Middle(int);
	void door_ChoiceRight(int);
	void door_ChoiceLeft(int);
	void door_DelayRight(int);
	void door_DelayLeft(int);

	void dispenseRight(int);
	void dispenseLeft(int);

	int delay_max;
	int delay_min;
	int delay_interval;

	int rew_amt;

	string loopStarter;

	bool initialized;

	void clearBuffer();

public:
	DNMS();
	DNMS(int, int);
	DNMS(int, int, int, int, int, int);
	~DNMS();

	void setParameters(int, int, int, int);

	void startExperiment();
	void endExperiment();

signals:
	QString toLog();

};

#endif
