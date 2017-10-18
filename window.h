#ifndef WINDOW_H
#define WINDOW_H

#include "defines.h"
#include "window.h"
#include "expthread.h"
#include "arduino.h"
#include "logger.h"

#include <QMainWindow>
#include <QCoreApplication>
#include <QLabel>
#include <QtGui>
#include <QBoxLayout>

#include <iostream>

using namespace std;

class QGroupBox;
class Arduino;
class Logger;
class expThread;

class MainWindow : public QWidget
{
	Q_OBJECT
	QThread workerThread;

public:
	MainWindow(QWidget *parent = 0);
	QGridLayout* mainGrid;

private:
	expThread myExp;
	Logger *log;

	bool checkTestState();

	QGroupBox *createTitle();
	QGroupBox *createOptions();
	QGroupBox *createExpParams_DNMS();
	QGroupBox *createExpParams_Rando();
	QGroupBox *createExpParams_Fish();
	QGroupBox *createLiveFeedSpace();
	QGroupBox *createLiveFeedFrame();

	QVBoxLayout *loggerVBox;
	QGroupBox *logPage;
	//QScrollArea *scrollArea;

	QList <QLabel *> label;

	void logData(string);

	void remove(QGridLayout*, int, int, bool);
	void deleteChildWidgets(QLayoutItem*);
	void removeRow(QGridLayout*, int, bool);
	void removeColumn(QGridLayout*, int, bool);

	void closeEvent(QCloseEvent *event);

	string DNMS_rat;
	int DNMS_rew, DNMS_trials, DNMS_ITE, DNMS_delMode;

	int ardTestState, ardPort, ardBaud, ardExp;
	int log_last, log_curr;

	bool expRunning;

public slots:
	void logSlot(QString, int);
	void DNMS_Start();
	void DNMS_End();


private slots:
	void testLogger();
	void refreshLog();

	void ardPortChanged(int);
	void ardBaudChanged(int);
	void expChanged(int);

	void testArd();

	void DNMS_textChanged_rat(QString);
	void DNMS_textChanged_rew(QString);
	void DNMS_textChanged_ite(QString);
	void DNMS_delMode1();
	void DNMS_delMode2();
	void DNMS_trials56();
	void DNMS_trials64();

signals:
	void startDNMS_exp(int, int, int, int, int, int);

};

#endif
