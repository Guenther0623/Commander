#include "defines.h"
#include "window.h"
#include "expthread.h"
#include "arduino.h"

#include <QCoreApplication>
#include <QMainWindow>
#include <QLabel>
#include <QtGui>
#include <QPushButton>
#include <QBoxLayout>
#include <QGridLayout>
#include <QWidget>

using namespace std;

/*
 *  MainTitle
 *
 *  Constructor for the MainWindow object
 *
 */
 
MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
{
	ardPort = _PORT_ACM0;
	ardBaud = _BAUD_9600;
	ardExp = _DNMS;
	ardTestState = _COMM_UNTESTED;
	expRunning = false;

	DNMS_rat  = "";
	DNMS_rew = 0;
	DNMS_delMode = _0X2X8X32;
	DNMS_trials = 56;

	mainGrid = new QGridLayout();

	mainGrid->addWidget(createTitle(), 0, 0, 1, 0);
	mainGrid->addWidget(createOptions(), 1, 0, 1, 0);
	mainGrid->addWidget(createExpParams_DNMS(), 2, 0);
	mainGrid->addWidget(createLiveFeedSpace(), 2, 1);
	mainGrid->addWidget(createLiveFeedFrame(), 2, 1);
	
	connect(&myExp, SIGNAL(sendComm(QString, int)), this, SLOT(logSlot(QString, int)));
	connect(&myExp, SIGNAL(updateWindow()), this, SLOT(logUpdate()));

	connect(this, SIGNAL(startDNMS_exp(int, int, int, int, int, int)), &myExp, SLOT(startDNMS(int, int, int, int, int, int)));

	setLayout(mainGrid);
	setWindowTitle(tr("Commander"));

	myExp.exp_running = false;
	myExp.moveToThread(&workerThread);
	workerThread.start();
}

/*
 *  createTitle
 *
 *  Creates the title box
 *
 */
QGroupBox *MainWindow::createTitle()
{
	QGroupBox *groupBox = new QGroupBox();
	groupBox->setStyleSheet("background-color: rgb(0, 128, 128)");

	QHBoxLayout *hbox = new QHBoxLayout;
	QVBoxLayout *vbox = new QVBoxLayout;

	QLabel *title = new QLabel("Commander V0.4A");
	QFont font("Arial", 20, QFont::Bold);
	title->setAlignment(Qt::AlignCenter);
	title->setFont(font);

	hbox->setAlignment(Qt::AlignCenter);
	hbox->addWidget(title);

	vbox->setAlignment(Qt::AlignCenter);
	vbox->addLayout(hbox);

	groupBox->setLayout(vbox);

	return groupBox;
}

/*
 *  createOptions
 *
 *  Creates the options box with arduino Test area
 *
 */
QGroupBox *MainWindow::createOptions()
{
	QGroupBox *groupBox = new QGroupBox(tr("Option Select Menu"));
	groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 20px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px}"));

	QVBoxLayout *vbox = new QVBoxLayout;
	QGridLayout *hbox = new QGridLayout;

	QGroupBox *groupBoxArd = new QGroupBox(tr("Arduino"));
	groupBoxArd->setStyleSheet(QString::fromUtf8("QGroupBox {border: 0; margin-top: 2ex; margin-left: 1ex; font-size: 18px} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 3px}"));
	QGridLayout *gridLayoutArd = new QGridLayout();
	QLabel *labelPort = new QLabel("Port: ");
	QLabel *labelBaud = new QLabel("Baudrate: ");
	QComboBox *buttonPort = new QComboBox;
	QComboBox *buttonBaud = new QComboBox;
	buttonPort->setFixedSize(200, 30);
	buttonBaud->setFixedSize(200, 30);

	gridLayoutArd->addWidget(labelPort, 0, 0);
	gridLayoutArd->addWidget(buttonPort, 0, 1);
	gridLayoutArd->addWidget(labelBaud, 1, 0);
	gridLayoutArd->addWidget(buttonBaud, 1, 1);
	groupBoxArd->setLayout(gridLayoutArd);

	buttonPort->addItem(tr("ttyACM0"));
	buttonPort->addItem(tr("ttyACM1"));
	buttonPort->addItem(tr("ttyUSB0"));
	buttonPort->addItem(tr("ttyUSB1"));
	connect(buttonPort, SIGNAL(activated(int)), this, SLOT(ardPortChanged(int)));

	buttonBaud->addItem(tr("9600"));
	buttonBaud->addItem(tr("19200"));
	buttonBaud->addItem(tr("4800"));
	connect(buttonBaud, SIGNAL(activated(int)), this, SLOT(ardBaudChanged(int)));

	QGroupBox *groupBoxExp = new QGroupBox(tr("Experiment"));
	groupBoxExp->setStyleSheet(QString::fromUtf8("QGroupBox {border: 0; margin-top: 2ex; margin-left: 1ex; font-size: 18px} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 3px}"));
	QGridLayout *gridLayoutExp = new QGridLayout();
	QLabel *labelExp = new QLabel("Select Experiment");
	QLabel *labelSaveLog = new QLabel("Save Log File");
	QComboBox *buttonExp = new QComboBox;
	QPushButton *buttonSaveLog = new QPushButton(tr(""));
	buttonExp->setFixedSize(200, 30);
	buttonSaveLog->setFixedSize(200, 30);

	gridLayoutExp->addWidget(labelExp, 0, 0);
	gridLayoutExp->addWidget(buttonExp, 0, 1);
	gridLayoutExp->addWidget(labelSaveLog, 1, 0);
	gridLayoutExp->addWidget(buttonSaveLog, 1, 1);
	groupBoxExp->setLayout(gridLayoutExp);

	buttonExp->addItem(tr("Spatial DNMS"));
	buttonExp->addItem(tr("Behavioral Random"));
	buttonExp->addItem(tr("Fishing Hole"));

	connect(buttonExp, SIGNAL(activated(int)), this, SLOT(expChanged(int)));

	hbox->addWidget(groupBoxArd, 0, 0);
	hbox->addWidget(groupBoxExp, 0, 1);
	vbox->addSpacerItem(new QSpacerItem(1, 10));
	vbox->addLayout(hbox);

	QLabel *connectLabel = new QLabel("Test Status:");
	QLabel *statusLabel = new QLabel("Untested");
	QFont font("Arial", 20, QFont::Bold);
	statusLabel->setFont(font);
	statusLabel->setStyleSheet("QLabel { color : orange; }");
	QPushButton *buttonTest = new QPushButton(tr("Test Arduino\n Connection "));
	buttonTest->setMinimumWidth(175);
	buttonTest->setMaximumWidth(175);
	buttonTest->setMinimumHeight(50);
	buttonTest->setMaximumHeight(50);
	connect(buttonTest, SIGNAL(released()), this, SLOT(testArd()));

	QHBoxLayout *hlay_sub1 = new QHBoxLayout;
	QHBoxLayout *hlay_sub2 = new QHBoxLayout;
	QHBoxLayout *hlay_testL = new QHBoxLayout;
	QVBoxLayout *vlay = new QVBoxLayout;
	QHBoxLayout *hlay = new QHBoxLayout;

	QLabel *testLabel = new QLabel("Arduino Commmunication Test Menu");
	QFont testFont("Arial", 16, QFont::Bold);	
	testLabel->setFont(testFont);

	hlay_sub1->addSpacerItem(new QSpacerItem(75, 10));
	hlay_sub1->addWidget(connectLabel);
	hlay_sub2->addSpacerItem(new QSpacerItem(55, 10));
	hlay_sub2->addWidget(statusLabel);
	vlay->addLayout(hlay_sub1);
	vlay->addLayout(hlay_sub2);

	hlay->addSpacerItem(new QSpacerItem(125, 10));
	hlay->addWidget(buttonTest);
	hlay->addSpacerItem(new QSpacerItem(40, 10));
	hlay->addLayout(vlay);

	hlay_testL->addSpacerItem(new QSpacerItem(120, 10));
	hlay_testL->addWidget(testLabel);

	vbox->addSpacerItem(new QSpacerItem(40, 10));
	vbox->addLayout(hlay_testL);
	vbox->addLayout(hlay);

	groupBox->setLayout(vbox);
	return groupBox;
}

/*
 *  createExpParams
 *
 *  Creates the experiment parameters box
 *
 */
QGroupBox *MainWindow::createExpParams_DNMS()
{
	QGroupBox *expParamsBox = new QGroupBox(tr("DNMS Experiment Parameters"));
	expParamsBox->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 18px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px}"));

	QHBoxLayout *hboxRat = new QHBoxLayout;
	QHBoxLayout *hboxRewardAmount = new QHBoxLayout;
	QHBoxLayout *hboxITE = new QHBoxLayout;
	
	QLabel *labelRat = new QLabel("Rat ID Number");
	QLineEdit *lineRat = new QLineEdit();
	QLabel *labelRewardAmount = new QLabel("Choice Reward");
	QLineEdit *lineRewardAmount = new QLineEdit();	
	QLabel *labelITE = new QLabel("ITI (in seconds)");
	QLineEdit *lineITE = new QLineEdit();

	lineRewardAmount->setValidator(new QIntValidator(lineRewardAmount));
	lineITE->setValidator(new QIntValidator(lineITE));
	
	lineRat->setFixedSize(100, 25);
	lineRewardAmount->setFixedSize(100, 25);
	lineITE->setFixedSize(100, 25);

	connect(lineRat, SIGNAL(textChanged(QString)), this, SLOT(DNMS_textChanged_rat(QString)));
	connect(lineRewardAmount, SIGNAL(textChanged(QString)), this, SLOT(DNMS_textChanged_rew(QString)));
	connect(lineITE, SIGNAL(textChanged(QString)), this, SLOT(DNMS_textChanged_ite(QString)));

	//QLabel *labelDelMode = new QLabel("Delay Mode Selection");
	QGroupBox *grpDelMode = new QGroupBox(tr("Delay Mode"));
	grpDelMode->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 15px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 3px}"));

	QRadioButton *delMode1 = new QRadioButton(tr("0, 2, 8, and 32 Seconds"));
	QRadioButton *delMode2 = new QRadioButton(tr("0, 4, 16, and 32 Seconds"));
	connect(delMode1, SIGNAL(released()), this, SLOT(DNMS_delMode1()));
	connect(delMode2, SIGNAL(released()), this, SLOT(DNMS_delMode2()));
	delMode1->setChecked(true);

	QVBoxLayout *vboxDelMode = new QVBoxLayout;
	vboxDelMode->addWidget(delMode1);
	vboxDelMode->addWidget(delMode2);

	grpDelMode->setLayout(vboxDelMode);

	QGroupBox *grpTrialNum = new QGroupBox(tr("Number of Trials"));
	grpTrialNum->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 15px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 3px}"));

	QRadioButton *trial56 = new QRadioButton(tr("56 Trials (112 Loops)"));
	QRadioButton *trial64 = new QRadioButton(tr("64 Trials (128 Loops)"));
	connect(trial56, SIGNAL(released()), this, SLOT(DNMS_trials56()));
	connect(trial64, SIGNAL(released()), this, SLOT(DNMS_trials64()));
	trial56->setChecked(true);

	QVBoxLayout *vboxTrialNum = new QVBoxLayout;
	vboxTrialNum->addWidget(trial56);
	vboxTrialNum->addWidget(trial64);

	grpTrialNum->setLayout(vboxTrialNum);

	lineRat->setAlignment(Qt::AlignRight);
	lineRewardAmount->setAlignment(Qt::AlignRight);
	lineITE->setAlignment(Qt::AlignRight);

	hboxRat->addWidget(labelRat);
	hboxRat->addWidget(lineRat);
	hboxRewardAmount->addWidget(labelRewardAmount);
	hboxRewardAmount->addWidget(lineRewardAmount);
	hboxITE->addWidget(labelITE);
	hboxITE->addWidget(lineITE);


	QPushButton *buttonStrt = new QPushButton("Start Experiment");
	QPushButton *buttonStop = new QPushButton("Stop Experiment");
	connect(buttonStrt, SIGNAL(released()), this, SLOT(DNMS_Start()));
	connect(buttonStop, SIGNAL(released()), this, SLOT(DNMS_End()));

	buttonStrt->setMinimumWidth(200);
	buttonStrt->setMaximumWidth(200);
	buttonStop->setMinimumWidth(200);
	buttonStop->setMaximumWidth(200);
	buttonStrt->setMinimumHeight(50);
	buttonStrt->setMaximumHeight(50);
	buttonStop->setMinimumHeight(50);
	buttonStrt->setMaximumHeight(50);

	QHBoxLayout *hboxStart = new QHBoxLayout;
	hboxStart->addSpacerItem(new QSpacerItem(60, 30));
	hboxStart->addWidget(buttonStrt);
	hboxStart->addSpacerItem(new QSpacerItem(60, 30));

	QHBoxLayout *hboxStop = new QHBoxLayout;
	hboxStop->addSpacerItem(new QSpacerItem(60, 30));
	hboxStop->addWidget(buttonStop);
	hboxStop->addSpacerItem(new QSpacerItem(60, 30));

	QVBoxLayout *vbox  = new QVBoxLayout;

	vbox->addSpacerItem(new QSpacerItem(325, 10));
	vbox->addLayout(hboxRat);
	vbox->addLayout(hboxRewardAmount);
	vbox->addLayout(hboxITE);
	vbox->addWidget(grpDelMode);
	vbox->addWidget(grpTrialNum);
	vbox->addLayout(hboxStart);
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxStop);

	expParamsBox->setLayout(vbox);

	return expParamsBox;
}

QGroupBox *MainWindow::createExpParams_Rando()
{
	QGroupBox *expParamsBox = new QGroupBox(tr("Random Experiment Parameters"));
	expParamsBox->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 18px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px}"));


	QVBoxLayout *vbox  = new QVBoxLayout;

	vbox->addSpacerItem(new QSpacerItem(325, 10));

	expParamsBox->setLayout(vbox);

	return expParamsBox;
}

QGroupBox *MainWindow::createExpParams_Fish()
{
	QGroupBox *expParamsBox = new QGroupBox(tr("Fishing Hole Experiment Parameters"));
	expParamsBox->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 18px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px}"));


	QVBoxLayout *vbox  = new QVBoxLayout;

	vbox->addSpacerItem(new QSpacerItem(325, 10));

	expParamsBox->setLayout(vbox);

	return expParamsBox;
}

/*
 * createLiveFeedSpace
 *
 * Creates the open space that is 
 * later populated by QScrollArea
 * created in createLiveFeedFrame
 *
 */

QGroupBox *MainWindow::createLiveFeedSpace()
{
	QGroupBox *groupBox = new QGroupBox();
	//QScrollArea *scrollArea = new QScrollArea();

	QVBoxLayout *spaceVBox = new QVBoxLayout();
	spaceVBox->addSpacerItem(new QSpacerItem(560, 400));

	groupBox->setLayout(spaceVBox);

	return groupBox;
}

/*
 *  createLiveFeed
 *
 *  Creates the live feed box
 *
 */
QGroupBox *MainWindow::createLiveFeedFrame()
{
	loggerVBox = new QVBoxLayout();

	QGroupBox* groupBox = new QGroupBox();
	groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {background-color: lightgray; border-style: solid; border-width: 3px; border-radius: 10px; border-color: gray; margin-top: 1ex; font-size: 20px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 10px}"));

	QHBoxLayout *tempHBox = new QHBoxLayout();
	QVBoxLayout *tempVBox = new QVBoxLayout();
	
	QWidget *widg = new QWidget();
	widg->setLayout(tempVBox);
	
	QScrollArea *scrollArea = new QScrollArea();
	scrollArea->setWidget(widg);

	tempHBox->addWidget(scrollArea);
	groupBox->setLayout(tempHBox);

	log_last = 0;
	log_curr = 0;
	
	return groupBox;
}

bool MainWindow::checkTestState()
{
	if(ardTestState == _COMM_UNTESTED)
	{
		QMessageBox* error = new QMessageBox();
		error->setText("Arduino Communication not Tested\nPlease test the Arduino Communication\nand try again");
		error->setWindowTitle("ERROR");
		error->show();
		return false;
	}
	else if(ardTestState == _COMM_WRONG || ardTestState == _COMM_MISSING)
	{
		QMessageBox* error = new QMessageBox();
		error->setText("Arduino Communication test Failed\nPlease retest the Arduino Communication\nand try again");
		error->setWindowTitle("ERROR");
		error->show();
		return false;
	}
	else
	{
		if(myExp.exp_running){
			QMessageBox* error = new QMessageBox();
			error->setText("Experiment is Currently Running\nPlease end Current Experiment\nand try again");
			error->setWindowTitle("ERROR");
			error->show();
			return false;
		}
		else{
			return true;
		}
	}
}


/*Logger Functions
 *
 *These functions take in QStrings and 
 *log the necessary data to the text
 *logger and live-feed window
 *
 */
void MainWindow::logData(string input)
{
	QString qstr = QString::fromStdString(input);
	QLabel *log = new QLabel(qstr);

	loggerVBox->addWidget(log, 0, 0);

	QGroupBox *groupBox = new QGroupBox();
	groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {background-color: lightgray; border-style: solid; border-width: 3px; border-radius: 10px; border-color: gray; margin-top: 1ex; font-size: 20px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 10px}"));

	QHBoxLayout *hbox = new QHBoxLayout();

	QWidget *widg = new QWidget();
	widg->setLayout(loggerVBox);

	QScrollArea *scrollArea = new QScrollArea();
	scrollArea->setWidget(widg);

	hbox->addWidget(scrollArea);
	groupBox->setLayout(hbox);

	scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());

	log_last = 0;
	log_curr = 0;

	mainGrid->addWidget(groupBox, 2, 1);
}

/*void MainWindow::logUpdate()
{
	
}*/

void MainWindow::refreshLog()
{
	remove(mainGrid, 2, 1, true);
	mainGrid->addWidget(createLiveFeedSpace(), 2, 1);
	mainGrid->addWidget(createLiveFeedFrame(), 2, 1);
}


/**
 *-----------------------------------------------------------
 *------------------Functions for SLOTS--------------------
 *-----------------------------------------------------------
 *-----------------------------------------------------------
 */
void MainWindow::expChanged(int index)
{
	switch(index){
		case 0:
			ardExp = _DNMS;
			remove(mainGrid, 2, 0, true);
			mainGrid->addWidget(createExpParams_DNMS(), 2, 0);
			break;

		case 1:
			ardExp = _RANDO;
			remove(mainGrid, 2, 0, true);
			mainGrid->addWidget(createExpParams_Rando(), 2, 0);
			break;

		case 2:
			ardExp = _FISH;
			remove(mainGrid, 2, 0, true);
			mainGrid->addWidget(createExpParams_Fish(), 2, 0);
			break;
	}
}

void MainWindow::ardPortChanged(int index)
{
	switch(index){
		case 0:
			ardPort = _PORT_ACM0;
			break;

		case 1:
			ardPort = _PORT_ACM1;
			break;

		case 2:
			ardPort = _PORT_USB0;
			break;

		case 3:
			ardPort = _PORT_USB1;
			break;
	}
}

void MainWindow::ardBaudChanged(int index)
{
	switch(index){
		case 0:
			ardBaud = _BAUD_9600;
			break;
		
		case 1:
			ardBaud = _BAUD_19200;
			break;

		case 2:
			ardBaud = _BAUD_4800;
			break;
	}
}

void MainWindow::testArd()
{
	int ardTest = _COMM_UNTESTED;
	Arduino* ard = new Arduino(ardPort, ardBaud);

	if(ard->ardExists)
	{
		ard->sendCommand("IDENTIFY");
		usleep(2000);
		if(ard->readCommand())
		{
			string expected = "Maurer Lab Arduino: DRF";
			if(ard->lastComm.compare(expected) == 0)
				ardTest = _COMM_SUCCEEDED;
			else
				ardTest = _COMM_WRONG;
		}
	}
	else
		ardTest = _COMM_MISSING;

	if(ardTest != ardTestState)
	{
		ardTestState = ardTest;
		remove(mainGrid, 1, 0, true);
		QGroupBox *groupBox = new QGroupBox(tr("Option Select Menu"));
		groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 20px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px}"));

		QVBoxLayout *vbox = new QVBoxLayout;
		QGridLayout *hbox = new QGridLayout;

		QGroupBox *groupBoxArd = new QGroupBox(tr("Arduino"));
		groupBoxArd->setStyleSheet(QString::fromUtf8("QGroupBox {border: 0; margin-top: 2ex; margin-left: 1ex; font-size: 18px} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 3px}"));
		QGridLayout *gridLayoutArd = new QGridLayout();
		QLabel *labelPort = new QLabel("Port: ");
		QLabel *labelBaud = new QLabel("Baudrate: ");
		QComboBox *buttonPort = new QComboBox;
		QComboBox *buttonBaud = new QComboBox;
		buttonPort->setFixedSize(200, 30);
		buttonBaud->setFixedSize(200, 30);

		gridLayoutArd->addWidget(labelPort, 0, 0);
		gridLayoutArd->addWidget(buttonPort, 0, 1);
		gridLayoutArd->addWidget(labelBaud, 1, 0);
		gridLayoutArd->addWidget(buttonBaud, 1, 1);
		groupBoxArd->setLayout(gridLayoutArd);

		buttonPort->addItem(tr("ttyACM0"));
		buttonPort->addItem(tr("ttyACM1"));
		buttonPort->addItem(tr("ttyUSB0"));
		buttonPort->addItem(tr("ttyUSB1"));

		if(ardPort == _PORT_ACM0){
			buttonPort->setCurrentIndex(0);
		}
		else if(ardPort == _PORT_ACM1){
			buttonPort->setCurrentIndex(1);
		}
		else if(ardPort == _PORT_USB0){
			buttonPort->setCurrentIndex(2);
		}
		else if(ardPort == _PORT_USB1){
			buttonPort->setCurrentIndex(3);
		}
		
		connect(buttonPort, SIGNAL(activated(int)), this, SLOT(ardPortChanged(int)));

		buttonBaud->addItem(tr("9600"));
		buttonBaud->addItem(tr("19200"));
		buttonBaud->addItem(tr("4800"));

		if(ardBaud == _BAUD_9600){
			buttonBaud->setCurrentIndex(0);
		}
		else if(ardBaud == _BAUD_19200){
			buttonBaud->setCurrentIndex(1);
		}
		else if(ardBaud == _BAUD_4800){
			buttonBaud->setCurrentIndex(2);
		}

		connect(buttonBaud, SIGNAL(activated(int)), this, SLOT(ardBaudChanged(int)));

		QGroupBox *groupBoxExp = new QGroupBox(tr("Experiment"));
		groupBoxExp->setStyleSheet(QString::fromUtf8("QGroupBox {border: 0; margin-top: 2ex; margin-left: 1ex; font-size: 18px} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 3px}"));
		QGridLayout *gridLayoutExp = new QGridLayout();
		QLabel *labelExp = new QLabel("Select Experiment");
		QLabel *labelSaveLog = new QLabel("Save Log File");
		QComboBox *buttonExp = new QComboBox;
		QPushButton *buttonSaveLog = new QPushButton(tr(""));
		buttonExp->setFixedSize(200, 30);
		buttonSaveLog->setFixedSize(200, 30);

		gridLayoutExp->addWidget(labelExp, 0, 0);
		gridLayoutExp->addWidget(buttonExp, 0, 1);
		gridLayoutExp->addWidget(labelSaveLog, 1, 0);
		gridLayoutExp->addWidget(buttonSaveLog, 1, 1);
		groupBoxExp->setLayout(gridLayoutExp);

		buttonExp->addItem(tr("Spatial DNMS"));
		buttonExp->addItem(tr("Behavioral Random"));
		buttonExp->addItem(tr("Fishing Hole"));

		if(ardExp == _DNMS){
			buttonExp->setCurrentIndex(0);
		}		
		else if(ardExp == _RANDO){
			buttonExp->setCurrentIndex(1);
		}
		else if(ardExp == _FISH){
			buttonExp->setCurrentIndex(2);
		}

		connect(buttonExp, SIGNAL(activated(int)), this, SLOT(expChanged(int)));

		hbox->addWidget(groupBoxArd, 0, 0);
		hbox->addWidget(groupBoxExp, 0, 1);
		vbox->addSpacerItem(new QSpacerItem(1, 10));
		vbox->addLayout(hbox);

		QLabel *connectLabel = new QLabel("Test Status:");
		QLabel *statusLabel;

		QPushButton *buttonTest = new QPushButton(tr("Test Arduino\n Connection "));
		buttonTest->setMinimumWidth(175);
		buttonTest->setMaximumWidth(175);
		buttonTest->setMinimumHeight(50);
		buttonTest->setMaximumHeight(50);
		connect(buttonTest, SIGNAL(released()), this, SLOT(testArd()));

		QHBoxLayout *hlay_sub1 = new QHBoxLayout;
		QHBoxLayout *hlay_sub2 = new QHBoxLayout;
		QHBoxLayout *hlay_testL = new QHBoxLayout;
		QVBoxLayout *vlay = new QVBoxLayout;
		QHBoxLayout *hlay = new QHBoxLayout;

		QLabel *testLabel = new QLabel("Arduino Commmunication Test Menu");
		QFont testFont("Arial", 16, QFont::Bold);	
		testLabel->setFont(testFont);

		if(ardTestState == _COMM_UNTESTED)
		{
			statusLabel = new QLabel("Untested");
			QFont font("Arial", 20, QFont::Bold);
			statusLabel->setFont(font);
			statusLabel->setStyleSheet("QLabel { color : orange; }");
			hlay_sub1->addSpacerItem(new QSpacerItem(75, 10));
			hlay_sub1->addWidget(connectLabel);
			hlay_sub2->addSpacerItem(new QSpacerItem(55, 10));
			hlay_sub2->addWidget(statusLabel);
		}
		else if(ardTestState == _COMM_MISSING)
		{
			statusLabel = new QLabel("Failed Connection");
			QFont font("Arial", 20, QFont::Bold);
			statusLabel->setFont(font);
			statusLabel->setStyleSheet("QLabel { color : red; }");
			hlay_sub1->addSpacerItem(new QSpacerItem(75, 10));
			hlay_sub1->addWidget(connectLabel);
			hlay_sub2->addSpacerItem(new QSpacerItem(20, 10));
			hlay_sub2->addWidget(statusLabel);
		}
		else if(ardTestState == _COMM_WRONG)
		{
			statusLabel = new QLabel("Wrong Message");
			QFont font("Arial", 20, QFont::Bold);
			statusLabel->setFont(font);
			statusLabel->setStyleSheet("QLabel { color : red; }");
			hlay_sub1->addSpacerItem(new QSpacerItem(75, 10));
			hlay_sub1->addWidget(connectLabel);
			hlay_sub2->addSpacerItem(new QSpacerItem(35, 10));
			hlay_sub2->addWidget(statusLabel);
		}
		else if(ardTestState == _COMM_SUCCEEDED)
		{
			statusLabel = new QLabel("Successful");
			QFont font("Arial", 20, QFont::Bold);
			statusLabel->setFont(font);
			statusLabel->setStyleSheet("QLabel { color : green; }");
			hlay_sub1->addSpacerItem(new QSpacerItem(75, 10));
			hlay_sub1->addWidget(connectLabel);
			hlay_sub2->addSpacerItem(new QSpacerItem(30, 10));
			hlay_sub2->addWidget(statusLabel);
		}

		vlay->addLayout(hlay_sub1);
		vlay->addLayout(hlay_sub2);

		hlay->addSpacerItem(new QSpacerItem(125, 10));
		hlay->addWidget(buttonTest);
		hlay->addSpacerItem(new QSpacerItem(40, 10));
		hlay->addLayout(vlay);

		hlay_testL->addSpacerItem(new QSpacerItem(120, 10));
		hlay_testL->addWidget(testLabel);

		vbox->addSpacerItem(new QSpacerItem(40, 10));
		vbox->addLayout(hlay_testL);
		vbox->addLayout(hlay);

		groupBox->setLayout(vbox);

		mainGrid->addWidget(groupBox, 1, 0, 1, 0);
	}

}

void MainWindow::DNMS_textChanged_rat(QString text)
{
	DNMS_rat = text.toStdString();
}

void MainWindow::DNMS_textChanged_rew(QString text)
{
	DNMS_rew = text.toInt();
}

void MainWindow::DNMS_textChanged_ite(QString text)
{
	DNMS_ITE = text.toInt();
}


void MainWindow::DNMS_delMode1()
{
	DNMS_trials = _0X2X8X32;
}

void MainWindow::DNMS_delMode2()
{
	DNMS_trials = _0X4X16X32;
}

void MainWindow::DNMS_trials56()
{
	DNMS_trials = 56;
}

void MainWindow::DNMS_trials64()
{
	DNMS_trials = 64;
}


void MainWindow::DNMS_Start()
{
	if(checkTestState()){
		log = new Logger(_DNMS, DNMS_rat);
		log->printTitle(DNMS_rew, DNMS_delMode, DNMS_ITE);
		emit startDNMS_exp(DNMS_rew, DNMS_trials, DNMS_ITE, DNMS_delMode, ardPort, ardBaud);
	}
}

void MainWindow::DNMS_End()
{
	if(myExp.exp_running)
		myExp.exp_running = false;

	else
	{
		QMessageBox* error = new QMessageBox();
		error->setText("No Experiment Currently Running");
		error->setWindowTitle("ERROR");
		error->show();
	}
}

void MainWindow::logSlot(QString in, int qLog)
{
	string toLog = in.toStdString();
	log->log(toLog, qLog);
	logData(toLog);
}

void MainWindow::testLogger()
{
	logData("hello");
}



/*
 * Helper function. Removes all layout items within the given @a layout
 * which either span the given @a row or @a column. If @a deleteWidgets
 * is true, all concerned child widgets become not only removed from the
 * layout, but also deleted.
 */
void MainWindow::remove(QGridLayout *layout, int row, int column, bool deleteWidgets) {
    // We avoid usage of QGridLayout::itemAtPosition() here to improve performance.
    for (int i = layout->count() - 1; i >= 0; i--) {
        int r, c, rs, cs;
        layout->getItemPosition(i, &r, &c, &rs, &cs);
        if ((r <= row && r + rs - 1 >= row) && (c <= column && c + cs - 1 >= column)) {
            // This layout item is subject to deletion.
            QLayoutItem *item = layout->takeAt(i);
            if (deleteWidgets) {
                deleteChildWidgets(item);
            }
            delete item;
        }
    }
}

/*
 * Helper function. Deletes all child widgets of the given layout @a item.
 */
void MainWindow::deleteChildWidgets(QLayoutItem *item) {
    if (item->layout()) {
        // Process all child items recursively.
        for (int i = 0; i < item->layout()->count(); i++) {
            deleteChildWidgets(item->layout()->itemAt(i));
        }
    }
    delete item->widget();
}

/*
 * Removes all layout items on the given @a row from the given grid
 * @a layout. If @a deleteWidgets is true, all concerned child widgets
 * become not only removed from the layout, but also deleted. Note that
 * this function doesn't actually remove the row itself from the grid
 * layout, as this isn't possible (i.e. the rowCount() and row indices
 * will stay the same after this function has been called).
 */
void MainWindow::removeRow(QGridLayout *layout, int row, bool deleteWidgets) {
    remove(layout, row, -1, deleteWidgets);
    layout->setRowMinimumHeight(row, 0);
    layout->setRowStretch(row, 0);
}

/*
 * Removes all layout items on the given @a column from the given grid
 * @a layout. If @a deleteWidgets is true, all concerned child widgets
 * become not only removed from the layout, but also deleted. Note that
 * this function doesn't actually remove the column itself from the grid
 * layout, as this isn't possible (i.e. the columnCount() and column
 * indices will stay the same after this function has been called).
 */
void MainWindow::removeColumn(QGridLayout *layout, int column, bool deleteWidgets) {
    remove(layout, -1, column, deleteWidgets);
    layout->setColumnMinimumWidth(column, 0);
    layout->setColumnStretch(column, 0);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	int resBtn = QMessageBox::question( this, "Close COnfirmation",
										"Are you sure you want to exit?\n",
										QMessageBox::Yes,
										QMessageBox::No);

	if(resBtn == QMessageBox::Yes){
		if(myExp.exp_running)
			myExp.exp_running = false;
		workerThread.quit();
		workerThread.wait();
		event->accept();
	}
	else if(resBtn == QMessageBox::No){
		event->ignore();
	}
}
