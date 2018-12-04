#ifndef CIRCLEMAZE_H
#define CIRCLEMAZE_H

#include <QComboBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLCDNumber>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QSpinBox>
#include <QtGui>
#include <QThread>
#include <QWidget>

#include "arduino.h"
#include "circleLogger.h"
#include "defines.h"
#include "expCircleThread.h"

#define _MODE_EXPER 1

class CircleMaze : public QWidget {
    Q_OBJECT
    QThread workerThread;

public:
    CircleMaze(QWidget *parent = nullptr);
    ~CircleMaze();

private:
    int ardPort, ardBaud, ardTestState;

    CircleLogger* log;
    expCircleThread myCircleExp;

    int startZone, zones, trials;

    QGroupBox *createTitle();
    QGroupBox *createOptions();
    QGroupBox *createExpParams();
    QGroupBox *createLogger();

    QGridLayout *mainGrid;

    QLabel *ardStatusLabel;
    QLabel *expStatusLabel;
    QLabel *expCurrentLabel;

    QLCDNumber *timerLCD;
    QTime time;
    QTimer *simulateTime;

    QTextEdit *logger;    

    void closeEvent(QCloseEvent*);

private slots:
    void updateValue_startZone(int);
    void updateValue_zones(QString);
    void updateValue_trials(QString);

    void experimentSTART();
    void requestExperimentEND();
    void experimentEND();

    void ardPortChanged(int);
    void ardBaudChanged(int);
    void testArd();
    bool isArdConnected();

    void addLog(QString, int);
    void clearLog();
    void tickTock();

signals:
    void StartCircle(int, int, int, int, int);
};

#endif // CIRCLEMAZE_H
