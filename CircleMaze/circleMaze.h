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
#include "defines.h"
#include "CircleMaze/expCircleThread.h"
#include "CircleMaze/circle_defines.h"
#include "circleLogger.h"


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

    int startZone[4];
    int zones[4];
    int trials[4];
    int ITI[4];
    string ratID[4];


    QGroupBox *createTitle();
    QGroupBox *createOptions();
    QGroupBox *createExpParams();
    QGroupBox *createLogger();

    QGroupBox *createExperiment();
    QGroupBox *createTraining1();
    QGroupBox *createTraining2();
    //QGroupBox *createTraining3();

    QLineEdit *lineDel1, *lineDel2, *lineDel3, *lineDel4;
    QLineEdit *lineDel5, *lineDel6, *lineDel7, *lineDel8;

    QGridLayout *mainGrid;

    QLabel *ardStatusLabel;
    QLabel *expStatusLabel;
    QLabel *expCurrentLabel;

    QLCDNumber *timerLCD;
    QTime time;
    QTimer *simulateTime;

    QTextEdit *logger;    

    int Circle_e_zones[8];
    int Circle_e_delNum;
    int Circle_e_delHolder;
    bool Circle_e_delIsCust;


    void closeEvent(QCloseEvent*);

private slots:

    void ratChanged_E1(QString);
    void trialsChanged_E1(QString);
    void sZoneChanged_E1(QString);

    void ratChanged_TP1(QString);
    void trialsChanged_TP1(QString);
    void sZoneChanged_TP1(QString);
    void sITIChanged_TP1(QString);

    void ratChanged_TP2(QString);
    void trialsChanged_TP2(QString);
    void sZoneChanged_TP2(QString);
    void sITIChanged_TP2(QString);

    void delaySpinChanged(int);

    void experimentSTART_E1();
    void experimentSTART_T1();
    void experimentSTART_T2();
    void requestExperimentEND();
    void experimentEND();

    void ardPortChanged(int);
    void ardBaudChanged(int);

    void testArd();
    bool isArdConnected();

    void addLog(QString, int);
    void clearLog();
    void tickTock();

    void circleL1Changed(QString);
    void circleL2Changed(QString);
    void circleL3Changed(QString);
    void circleL4Changed(QString);
    void circleL5Changed(QString);
    void circleL6Changed(QString);
    void circleL7Changed(QString);
    void circleL8Changed(QString);

signals:
    void StartCircle_Exp(int, int, int*, int, int);
    void StartCircle_T1(int, int, int, int, int);
    void StartCircle_T2(int, int, int, int, int);
};

#endif // CIRCLEMAZE_H
