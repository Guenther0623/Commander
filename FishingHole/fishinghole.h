#ifndef FISHINGHOLE_H
#define FISHINGHOLE_H

#include <string>
#include <QBoxLayout>
#include <QCoreApplication>
#include <QGridLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QtGui>
#include <QWidget>

#include "arduino.h"
#include "defines.h"
#include "FishingHole/expFishThread.h"
#include "FishingHole/fish_defines.h"
#include "fishLogger.h"

class QGroupBox;
class QTextEdit;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class FishLogger;
class expFishThread;

class FishingHole : public QWidget
{
	Q_OBJECT
	QThread workerThread;

	public:
		FishingHole(QWidget *parent = 0);
		~FishingHole();

	private:
		int ardPort, ardBaud, ardTestState;

		FishLogger *log;
		expFishThread myFishExp;

		std::string ratID[4];
		int trials[4], iti[4];
		int pMode; // probability mode

		QGroupBox *createTitle();
		QGroupBox *createOptions();
		QGroupBox *createExpParams();
		QGroupBox *createLogger();

		QGroupBox *createTraining1();
		QGroupBox *createTraining2();
		QGroupBox *createTraining3();
		QGroupBox *createExperiment();

		QGridLayout *mainGrid;

		QLabel *ardStatusLabel;
		QLabel *expStatusLabel;
		QLabel *expCurrentLabel;

		QLCDNumber *timerLCD;
		QTime time;
		QTimer *simulateTime;

		QTextEdit *logger;

		void closeEvent(QCloseEvent *event);
		
	private slots:
		void ratChanged_TP1(QString);
		void itiChanged_TP1(QString);
		void trialsChanged_TP1(QString);
		void ratChanged_TP2(QString);
		void itiChanged_TP2(QString);
		void trialsChanged_TP2(QString);
		void ratChanged_TP3(QString);
		void itiChanged_TP3(QString);
		void trialsChanged_TP3(QString);
		void ratChanged_E1(QString);
		void itiChanged_E1(QString);
		void trialsChanged_E1(QString);
		void predictiveChanged(bool);
		void nonpredChanged(bool);
		void fiftyChanged(bool);

		void ardPortChanged(int);
		void ardBaudChanged(int);

		void experimentSTART_T1();
		void experimentSTART_T2();
		void experimentSTART_T3();
		void experimentSTART_E1();
		void requestExperimentEND();
		void experimentEND();

		void testArd();
		bool isArdConnected();
		
		void addLog(QString, int);
		void clearLog();
		void tickTock();

	signals:
		void StartFish_T1(int, int, int, int);
		void StartFish_T2(int, int, int, int);
		void StartFish_T3(int, int, int, int);
		void StartFish_Exp(int, int, int, int, int);

};

#endif //FISHINGHOLE_H