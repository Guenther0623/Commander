#include "fishinghole.h"

using namespace std;

FishingHole::FishingHole(QWidget *parent)
	: QWidget(parent)
{
	mainGrid = new QGridLayout;

	time = QTime(0, 0, 0);

	pMode = 0;

	// defailt iti to 0
	for(int i = 0; i < 4; i++) {
		iti[i] = 0;
	}

	// defailt trials to 0
	for(int i = 0; i < 4; i++) {
		trials[i] = 0;
	}

	ardPort = _PORT_ACM0;
	ardBaud = _BAUD_9600;
	ardTestState = _COMM_UNTESTED;

	mainGrid->addWidget(createTitle(), 0, 0, 1, 0);
	mainGrid->addWidget(createOptions(), 1, 0, 1, 0);
	mainGrid->addWidget(createExpParams(), 2, 0);
	mainGrid->addWidget(createLogger(), 2, 1);

	connect(&myFishExp, SIGNAL(sendComm(QString, int)),
			this, SLOT(addLog(QString, int)));

	connect(&myFishExp, SIGNAL(naturalEnd()),
			this, SLOT(experimentEND()));

	connect(this, SIGNAL(StartFish_T1(int, int, int, int)), 
			&myFishExp, SLOT(startFish_Tr1(int, int, int, int)));

	connect(this, SIGNAL(StartFish_T2(int, int, int, int)), 
			&myFishExp, SLOT(startFish_Tr2(int, int, int, int)));

	connect(this, SIGNAL(StartFish_T3(int, int, int, int)), 
			&myFishExp, SLOT(startFish_Tr3(int, int, int, int)));

	connect(this, SIGNAL(StartFish_Exp(int, int, int, int, int)), 
			&myFishExp, SLOT(startFish_Exp(int, int, int, int, int)));

	setLayout(mainGrid);
	setWindowTitle(tr("Commander - Fishing Hole"));

	myFishExp.exp_running = false;
	myFishExp.moveToThread(&workerThread);
	workerThread.start();

}

FishingHole::~FishingHole()
{

}

/*
 *  createTitle
 *
 *  Creates the title box
 *
 */
QGroupBox *FishingHole::createTitle()
{
	QGroupBox *groupBox = new QGroupBox();
	groupBox->setStyleSheet("background-color: rgb(0, 128, 128)");

	QHBoxLayout *hbox = new QHBoxLayout;
	QVBoxLayout *vbox = new QVBoxLayout;

	QLabel *title = new QLabel("Commander V1.0A");
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
QGroupBox *FishingHole::createOptions()
{
	QGroupBox *groupBox = new QGroupBox(tr("Option Select Menu"));
	groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 20px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px}"));

	QVBoxLayout *vbox = new QVBoxLayout;
	QHBoxLayout *hbox = new QHBoxLayout;
	//QGridLayout *hbox = new QGridLayout;

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

	buttonBaud->addItem(tr("4800"));
	buttonBaud->addItem(tr("9600"));
	buttonBaud->addItem(tr("19200"));
	buttonBaud->setCurrentIndex(1);
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

	//buttonExp->addItem(tr("Spatial DNMS"));
	//buttonExp->addItem(tr("Delay Discounting"));
	//buttonExp->addItem(tr("Behavioral Random"));
	buttonExp->addItem(tr("Fishing Hole"));
	//buttonExp->addItem(tr("Smoke Monitor"));

	//connect(buttonExp, SIGNAL(activated(int)), this, SLOT(expChanged(int)));

	hbox->addStretch(100);
	hbox->addWidget(groupBoxArd);
	hbox->addStretch(100);
	hbox->addWidget(groupBoxExp);
	hbox->addStretch(100);
	vbox->addSpacerItem(new QSpacerItem(1, 10));
	vbox->addLayout(hbox);

	QLabel *connectLabel = new QLabel("Test Status:");
	ardStatusLabel = new QLabel("Untested");
	QFont font("Arial", 20, QFont::Bold);
	ardStatusLabel->setFont(font);
	ardStatusLabel->setStyleSheet("QLabel { color : orange; }");
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

	hlay_sub1->addStretch(100);		//SpacerItem(new QSpacerItem(75, 10));
	hlay_sub1->addWidget(connectLabel);
	hlay_sub1->addStretch(100);
	hlay_sub2->addStretch(100);
	hlay_sub2->addWidget(ardStatusLabel);
	hlay_sub2->addStretch(100);
	vlay->addLayout(hlay_sub1);
	vlay->addLayout(hlay_sub2);

	hlay->addStretch(100);
	//hlay->addSpacerItem(new QSpacerItem(125, 10));
	hlay->addWidget(buttonTest);
	hlay->addStretch(10);
	//hlay->addSpacerItem(new QSpacerItem(40, 10));
	hlay->addLayout(vlay);
	hlay->addStretch(100);

	hlay_testL->addStretch(100);
	hlay_testL->addWidget(testLabel);
	hlay_testL->addStretch(100);

	vbox->addSpacerItem(new QSpacerItem(40, 10));
	vbox->addLayout(hlay_testL);
	vbox->addLayout(hlay);

	groupBox->setLayout(vbox);
	return groupBox;
}

/*
 *
 *  createParamOptions
 *
 *  Creates the experimental buttons and parameter entries
 *  for use in experiment
 *
 */
QGroupBox *FishingHole::createExpParams()
{
	QGroupBox *groupBox = new QGroupBox(tr("Experiments"));
	groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 18px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px}"));

	//QGroupBox *exp_train1 = new QGroupBox;

	QTabWidget *paramTabs = new QTabWidget;
	paramTabs->addTab(createExperiment(), "Experiment");
	paramTabs->addTab(createTraining1(), "Training P1");
	paramTabs->addTab(createTraining2(), "Training P2");
	paramTabs->addTab(createTraining3(), "Training P3");

	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(paramTabs);

	groupBox->setLayout(hbox);
	return groupBox;
}

QGroupBox *FishingHole::createTraining1()
{
	QGroupBox *groupBox = new QGroupBox;

	QLabel *labelRat = new QLabel("Rat ID");
	QLabel *labelITI = new QLabel("ITI");
	QLineEdit *lineRat = new QLineEdit();
	lineRat->setFixedSize(100, 25);
	lineRat->setAlignment(Qt::AlignRight);
	QLineEdit *lineITI = new QLineEdit();
	lineITI->setFixedSize(100, 25);
	lineITI->setValidator(new QIntValidator(lineITI));
	lineITI->setAlignment(Qt::AlignRight);

	QHBoxLayout *hboxRat = new QHBoxLayout;
	hboxRat->addWidget(labelRat);
	hboxRat->addWidget(lineRat);
	QHBoxLayout *hboxITI = new QHBoxLayout;
	hboxITI->addWidget(labelITI);
	hboxITI->addWidget(lineITI);

	QLabel *labelTrials = new QLabel("Trial Count");
	QLineEdit *lineTrials = new QLineEdit();
	lineTrials->setFixedSize(100, 25);
	lineTrials->setValidator(new QIntValidator(lineTrials));
	lineTrials->setAlignment(Qt::AlignRight);
	QHBoxLayout *hboxTrials = new QHBoxLayout;
	hboxTrials->addWidget(labelTrials);
	hboxTrials->addWidget(lineTrials);

	QPushButton *buttonStrt = new QPushButton("Start");
	buttonStrt->setFixedSize(200, 50);
	QPushButton *buttonStop = new QPushButton("Stop");
	buttonStop->setFixedSize(200, 50);
	
	QHBoxLayout *hboxStart = new QHBoxLayout;
	hboxStart->addSpacerItem(new QSpacerItem(60, 20));
	hboxStart->addWidget(buttonStrt);
	hboxStart->addSpacerItem(new QSpacerItem(60, 20));

	QHBoxLayout *hboxStop = new QHBoxLayout;
	hboxStop->addSpacerItem(new QSpacerItem(60, 20));
	hboxStop->addWidget(buttonStop);
	hboxStop->addSpacerItem(new QSpacerItem(60, 20));

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxRat);
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxITI);
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxTrials);
	vbox->addStretch(100);
	vbox->addLayout(hboxStart);
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxStop);
	vbox->addSpacerItem(new QSpacerItem(20, 10));

	connect(lineRat, SIGNAL(textChanged(QString)),
			this, SLOT(ratChanged_TP1(QString)));
	connect(lineITI, SIGNAL(textChanged(QString)),
			this, SLOT(itiChanged_TP1(QString)));
	connect(lineTrials, SIGNAL(textChanged(QString)),
			this, SLOT(trialsChanged_TP1(QString)));
	connect(buttonStrt, SIGNAL(released()),
			this, SLOT(experimentSTART_T1()));
	connect(buttonStop, SIGNAL(released()), 
			this, SLOT(requestExperimentEND()));

	groupBox->setLayout(vbox);
	return groupBox;
}

QGroupBox *FishingHole::createTraining2()
{
	QGroupBox *groupBox = new QGroupBox;

	QLabel *labelRat = new QLabel("Rat ID");
	QLabel *labelITI = new QLabel("ITI");
	QLineEdit *lineRat = new QLineEdit();
	lineRat->setFixedSize(100, 25);
	lineRat->setAlignment(Qt::AlignRight);
	QLineEdit *lineITI = new QLineEdit();
	lineITI->setFixedSize(100, 25);
	lineITI->setValidator(new QIntValidator(lineITI));
	lineITI->setAlignment(Qt::AlignRight);

	QHBoxLayout *hboxRat = new QHBoxLayout;
	hboxRat->addWidget(labelRat);
	hboxRat->addWidget(lineRat);
	QHBoxLayout *hboxITI = new QHBoxLayout;
	hboxITI->addWidget(labelITI);
	hboxITI->addWidget(lineITI);

	QLabel *labelTrials = new QLabel("Trial Count");
	QLineEdit *lineTrials = new QLineEdit();
	lineTrials->setFixedSize(100, 25);
	lineTrials->setValidator(new QIntValidator(lineTrials));
	lineTrials->setAlignment(Qt::AlignRight);
	QHBoxLayout *hboxTrials = new QHBoxLayout;
	hboxTrials->addWidget(labelTrials);
	hboxTrials->addWidget(lineTrials);

	QPushButton *buttonStrt = new QPushButton("Start");
	buttonStrt->setFixedSize(200, 50);
	QPushButton *buttonStop = new QPushButton("Stop");
	buttonStop->setFixedSize(200, 50);
	
	QHBoxLayout *hboxStart = new QHBoxLayout;
	hboxStart->addSpacerItem(new QSpacerItem(60, 20));
	hboxStart->addWidget(buttonStrt);
	hboxStart->addSpacerItem(new QSpacerItem(60, 20));

	QHBoxLayout *hboxStop = new QHBoxLayout;
	hboxStop->addSpacerItem(new QSpacerItem(60, 20));
	hboxStop->addWidget(buttonStop);
	hboxStop->addSpacerItem(new QSpacerItem(60, 20));

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxRat);
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxITI);
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxTrials);
	vbox->addStretch(100);
	vbox->addLayout(hboxStart);
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxStop);
	vbox->addSpacerItem(new QSpacerItem(20, 10));

	connect(lineRat, SIGNAL(textChanged(QString)),
			this, SLOT(ratChanged_TP2(QString)));
	connect(lineITI, SIGNAL(textChanged(QString)),
			this, SLOT(itiChanged_TP2(QString)));
	connect(lineTrials, SIGNAL(textChanged(QString)),
			this, SLOT(trialsChanged_TP2(QString)));
	connect(buttonStrt, SIGNAL(released()),
			this, SLOT(experimentSTART_T2()));
	connect(buttonStop, SIGNAL(released()), 
			this, SLOT(requestExperimentEND()));

	groupBox->setLayout(vbox);
	return groupBox;
}

QGroupBox *FishingHole::createTraining3()
{
	QGroupBox *groupBox = new QGroupBox;

	QLabel *labelRat = new QLabel("Rat ID");
	QLabel *labelITI = new QLabel("ITI");
	QLineEdit *lineRat = new QLineEdit();
	lineRat->setFixedSize(100, 25);
	lineRat->setAlignment(Qt::AlignRight);
	QLineEdit *lineITI = new QLineEdit();
	lineITI->setFixedSize(100, 25);
	lineITI->setValidator(new QIntValidator(lineITI));
	lineITI->setAlignment(Qt::AlignRight);

	QHBoxLayout *hboxRat = new QHBoxLayout;
	hboxRat->addWidget(labelRat);
	hboxRat->addWidget(lineRat);
	QHBoxLayout *hboxITI = new QHBoxLayout;
	hboxITI->addWidget(labelITI);
	hboxITI->addWidget(lineITI);

	QLabel *labelTrials = new QLabel("Trial Count");
	QLineEdit *lineTrials = new QLineEdit();
	lineTrials->setFixedSize(100, 25);
	lineTrials->setValidator(new QIntValidator(lineTrials));
	lineTrials->setAlignment(Qt::AlignRight);
	QHBoxLayout *hboxTrials = new QHBoxLayout;
	hboxTrials->addWidget(labelTrials);
	hboxTrials->addWidget(lineTrials);

	QPushButton *buttonStrt = new QPushButton("Start");
	buttonStrt->setFixedSize(200, 50);
	QPushButton *buttonStop = new QPushButton("Stop");
	buttonStop->setFixedSize(200, 50);
	
	QHBoxLayout *hboxStart = new QHBoxLayout;
	hboxStart->addSpacerItem(new QSpacerItem(60, 20));
	hboxStart->addWidget(buttonStrt);
	hboxStart->addSpacerItem(new QSpacerItem(60, 20));

	QHBoxLayout *hboxStop = new QHBoxLayout;
	hboxStop->addSpacerItem(new QSpacerItem(60, 20));
	hboxStop->addWidget(buttonStop);
	hboxStop->addSpacerItem(new QSpacerItem(60, 20));

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxRat);
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxITI);
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxTrials);
	vbox->addStretch(100);
	vbox->addLayout(hboxStart);
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxStop);
	vbox->addSpacerItem(new QSpacerItem(20, 10));

	connect(lineRat, SIGNAL(textChanged(QString)),
			this, SLOT(ratChanged_TP3(QString)));
	connect(lineITI, SIGNAL(textChanged(QString)),
			this, SLOT(itiChanged_TP3(QString)));
	connect(lineTrials, SIGNAL(textChanged(QString)),
			this, SLOT(trialsChanged_TP3(QString)));
	connect(buttonStrt, SIGNAL(released()),
			this, SLOT(experimentSTART_T3()));
	connect(buttonStop, SIGNAL(released()), 
			this, SLOT(requestExperimentEND()));

	groupBox->setLayout(vbox);
	return groupBox;
}

QGroupBox *FishingHole::createExperiment()
{
	QGroupBox *groupBox = new QGroupBox;

	QLabel *labelRat = new QLabel("Rat ID");
	QLabel *labelITI = new QLabel("ITI");
	QLineEdit *lineRat = new QLineEdit();
	lineRat->setFixedSize(100, 25);
	lineRat->setAlignment(Qt::AlignRight);
	QLineEdit *lineITI = new QLineEdit();
	lineITI->setFixedSize(100, 25);
	lineITI->setValidator(new QIntValidator(lineITI));
	lineITI->setAlignment(Qt::AlignRight);

	QHBoxLayout *hboxRat = new QHBoxLayout;
	hboxRat->addWidget(labelRat);
	hboxRat->addWidget(lineRat);
	QHBoxLayout *hboxITI = new QHBoxLayout;
	hboxITI->addWidget(labelITI);
	hboxITI->addWidget(lineITI);

	QLabel *labelTrials = new QLabel("Trial Count");
	QLineEdit *lineTrials = new QLineEdit();
	lineTrials->setFixedSize(100, 25);
	lineTrials->setValidator(new QIntValidator(lineTrials));
	lineTrials->setAlignment(Qt::AlignRight);
	QHBoxLayout *hboxTrials = new QHBoxLayout;
	hboxTrials->addWidget(labelTrials);
	hboxTrials->addWidget(lineTrials);

	QGroupBox *probabilityOptions = new QGroupBox(tr("Probability Set"));
	probabilityOptions->setAlignment(Qt::AlignCenter);
	QRadioButton *nonPred = new QRadioButton("Non-Predictive");
	nonPred->setChecked(true);
	connect(nonPred, SIGNAL(toggled(bool)), this, SLOT(nonpredChanged(bool)));

	QRadioButton *predictive = new QRadioButton("Predictive");
	connect(predictive, SIGNAL(toggled(bool)), this, SLOT(predictiveChanged(bool)));

	QRadioButton *fiftyFifty = new QRadioButton("Only 50/50");
	connect(fiftyFifty, SIGNAL(toggled(bool)), this, SLOT(fiftyChanged(bool)));

	QVBoxLayout *vboxP = new QVBoxLayout;
    vboxP->addWidget(nonPred);
	vboxP->addWidget(predictive);
    vboxP->addWidget(fiftyFifty);
    vboxP->addStretch(1);
    probabilityOptions->setLayout(vboxP);

	QPushButton *buttonStrt = new QPushButton("Start");
	buttonStrt->setFixedSize(200, 50);
	QPushButton *buttonStop = new QPushButton("Stop");
	buttonStop->setFixedSize(200, 50);
	
	QHBoxLayout *hboxStart = new QHBoxLayout;
	hboxStart->addSpacerItem(new QSpacerItem(60, 20));
	hboxStart->addWidget(buttonStrt);
	hboxStart->addSpacerItem(new QSpacerItem(60, 20));

	QHBoxLayout *hboxStop = new QHBoxLayout;
	hboxStop->addSpacerItem(new QSpacerItem(60, 20));
	hboxStop->addWidget(buttonStop);
	hboxStop->addSpacerItem(new QSpacerItem(60, 20));

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxRat);
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxITI);
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxTrials);
	vbox->addWidget(probabilityOptions);
	vbox->addStretch(100);
	vbox->addLayout(hboxStart);
	vbox->addSpacerItem(new QSpacerItem(20, 10));
	vbox->addLayout(hboxStop);
	vbox->addSpacerItem(new QSpacerItem(20, 10));

	connect(lineRat, SIGNAL(textChanged(QString)),
			this, SLOT(ratChanged_E1(QString)));
	connect(lineITI, SIGNAL(textChanged(QString)),
			this, SLOT(itiChanged_E1(QString)));
	connect(lineTrials, SIGNAL(textChanged(QString)),
			this, SLOT(trialsChanged_E1(QString)));
	connect(buttonStrt, SIGNAL(released()),
			this, SLOT(experimentSTART_E1()));
	connect(buttonStop, SIGNAL(released()), 
			this, SLOT(requestExperimentEND()));

	groupBox->setLayout(vbox);
	return groupBox;
}

QGroupBox *FishingHole::createLogger()
{
	QGroupBox *groupBox = new QGroupBox(tr("Data Log Window"));
	groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 18px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px}"));

	expStatusLabel = new QLabel("Stopped");
	expCurrentLabel = new QLabel("N/A");
	QFont font("Arial", 14, QFont::Bold);
	expStatusLabel->setFont(font);
	expCurrentLabel->setFont(font);
	expStatusLabel->setStyleSheet("QLabel { color : red; }");
	expCurrentLabel->setStyleSheet("QLabel { color : red; }");

	QLabel *expQLabel = new QLabel("Status");
	QLabel *expRLabel = new QLabel("Experiment");

	QVBoxLayout *vlay_status = new QVBoxLayout;
	QHBoxLayout *hlay_sub1 = new QHBoxLayout;
	QHBoxLayout *hlay_sub2 = new QHBoxLayout;
	hlay_sub1->addStretch(100);		//SpacerItem(new QSpacerItem(75, 10));
	hlay_sub1->addWidget(expQLabel);
	hlay_sub1->addStretch(100);
	hlay_sub2->addStretch(100);
	hlay_sub2->addWidget(expStatusLabel);
	hlay_sub2->addStretch(100);
	vlay_status->addLayout(hlay_sub1);
	vlay_status->addLayout(hlay_sub2);

	QVBoxLayout *vlay_current = new QVBoxLayout;
	QHBoxLayout *hlay_sub3 = new QHBoxLayout;
	QHBoxLayout *hlay_sub4 = new QHBoxLayout;
	hlay_sub3->addStretch(100);		//SpacerItem(new QSpacerItem(75, 10));
	hlay_sub3->addWidget(expRLabel);

	hlay_sub3->addStretch(100);
	hlay_sub4->addStretch(100);
	hlay_sub4->addWidget(expCurrentLabel);
	hlay_sub4->addStretch(100);
	vlay_current->addLayout(hlay_sub3);
	vlay_current->addLayout(hlay_sub4);

	timerLCD = new QLCDNumber;
	timerLCD->setDigitCount(8);
	timerLCD->setSegmentStyle(QLCDNumber::Flat);
	timerLCD->setStyleSheet("QLCDNumber { border: none}");
	timerLCD->display("00:00:00");

	simulateTime = new QTimer;
	connect(simulateTime, SIGNAL(timeout()), this, SLOT(tickTock()));

	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(timerLCD);
	hbox->addLayout(vlay_current);
	hbox->addLayout(vlay_status);

	QVBoxLayout *vbox = new QVBoxLayout;
	logger = new QTextEdit;
	logger->setReadOnly(true);
	//logger->
	vbox->addLayout(hbox);
	vbox->addWidget(logger);

	groupBox->setLayout(vbox);

	return groupBox;
}

void FishingHole::testArd()
{
	int ardTest = _COMM_UNTESTED;
	Arduino* ard = new Arduino(ardPort, ardBaud);

	if(ard->ardExists)
	{
		ard->sendCommand("Iden");
		usleep(2000);
		if(ard->readCommand())
		{
			string expected = "Maurer Lab Arduino: DRF";
			if(ard->lastComm.compare(expected) == 0)
				ardTest = _COMM_SUCCEEDED;
			else
				ardTest = _COMM_WRONG;
		}
		else
			ardTest = _COMM_WRONG;
	}
	else
		ardTest = _COMM_MISSING;

	if(ardTest != ardTestState)
	{
		ardTestState = ardTest;

		if(ardTest == _COMM_MISSING)
		{
			ardStatusLabel->setText("Missing");
			ardStatusLabel->setStyleSheet("QLabel { color : red; }");
		}
		else if(ardTest == _COMM_WRONG)
		{
			ardStatusLabel->setText("Bad Message");
			ardStatusLabel->setStyleSheet("QLabel { color : red; }");
		}
		else if(ardTest == _COMM_SUCCEEDED)
		{
			ardStatusLabel->setText("Successful");
			ardStatusLabel->setStyleSheet("QLabel { color : green; }");
		}
	}
}

bool FishingHole::isArdConnected() {
	if (ardTestState == _COMM_UNTESTED)
	{
		QMessageBox* error = new QMessageBox();
		error->setText("Arduino Communication not Tested\nPlease test the Arduino Communication\nand try again");
		error->setWindowTitle("ERROR");
		error->show();
		return false;
	}
	else if (ardTestState == _COMM_WRONG || ardTestState == _COMM_MISSING)
	{
		QMessageBox* error = new QMessageBox();
		error->setText("Arduino Communication test Failed\nPlease retest the Arduino Communication\nand try again");
		error->setWindowTitle("ERROR");
		error->show();
		return false;
	}

	return true;
}

void FishingHole::ardPortChanged(int index)
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

void FishingHole::ardBaudChanged(int index)
{
	switch(index){
		case 0:
			ardBaud = _BAUD_4800;
			break;
		
		case 1:
			ardBaud = _BAUD_9600;
			break;

		case 2:
			ardBaud = _BAUD_19200;
			break;
	}
}

void FishingHole::ratChanged_TP1(QString text)
{	ratID[_MODE_TRAIN01] = text.toStdString();	}

void FishingHole::itiChanged_TP1(QString text)
{	iti[_MODE_TRAIN01] = text.toInt();	}

void FishingHole::trialsChanged_TP1(QString text)
{	trials[_MODE_TRAIN01] = text.toInt();	}

void FishingHole::ratChanged_TP2(QString text)
{	ratID[_MODE_TRAIN02] = text.toStdString();	}

void FishingHole::itiChanged_TP2(QString text)
{	iti[_MODE_TRAIN02] = text.toInt();	}

void FishingHole::trialsChanged_TP2(QString text)
{	trials[_MODE_TRAIN02] = text.toInt();	}

void FishingHole::ratChanged_TP3(QString text)
{	ratID[_MODE_TRAIN03] = text.toStdString();	}

void FishingHole::itiChanged_TP3(QString text)
{	iti[_MODE_TRAIN03] = text.toInt();	}

void FishingHole::trialsChanged_TP3(QString text)
{	trials[_MODE_TRAIN03] = text.toInt();	}

void FishingHole::ratChanged_E1(QString text)
{	ratID[_MODE_EXPER01] = text.toStdString();	}

void FishingHole::itiChanged_E1(QString text)
{	iti[_MODE_EXPER01] = text.toInt();	}

void FishingHole::trialsChanged_E1(QString text)
{	trials[_MODE_EXPER01] = text.toInt();	}

void FishingHole::predictiveChanged(bool checked)
{	if(checked) {
		pMode = _PREDICTIVE;
		cout << "pmode: " << pMode << endl;
	}
}

void FishingHole::nonpredChanged(bool checked)
{	if(checked) {
		pMode = _NP_ALL;
		cout << "pmode: " << pMode << endl;
	}
}

void FishingHole::fiftyChanged(bool checked)
{	if(checked) {
		pMode = _FIFTYFIFTY;
		cout << "pmode: " << pMode << endl;
	}
}


void FishingHole::experimentSTART_T1()
{
	if (!isArdConnected()){
		return;
	}
	if(myFishExp.exp_running){
		QMessageBox *error = new QMessageBox();
		error->setText("Experiment already running\nPlease stop current experiment before\nstarting a new one");
		error->setWindowTitle("ERROR");
		error->show();
		return;
	}	
	expStatusLabel->setText("Running");
	expCurrentLabel->setText("Training P1");
	expStatusLabel->setStyleSheet("QLabel { color : green; }");
	expCurrentLabel->setStyleSheet("QLabel { color : green; }");

	log = new FishLogger(_MODE_TRAIN01, ratID[_MODE_TRAIN01]);
	int tri_t = trials[_MODE_TRAIN01];
	int iti_t = iti[_MODE_TRAIN01];
	log->printTitle(tri_t, iti_t, -1);
	emit StartFish_T1(tri_t, iti_t, ardPort, ardBaud);

	logger->clear();
	timerLCD->display("00:00:00");
	time.setHMS(0, 0, 0);
	simulateTime->start(1000);
}

void FishingHole::experimentSTART_T2()
{
	if (!isArdConnected()){
		return;
	}
	if(myFishExp.exp_running){
		QMessageBox *error = new QMessageBox();
		error->setText("Experiment already running\nPlease stop current experiment before\nstarting a new one");
		error->setWindowTitle("ERROR");
		error->show();
		return;
	}
	expStatusLabel->setText("Running");
	expCurrentLabel->setText("Training P2");
	expStatusLabel->setStyleSheet("QLabel { color : green; }");
	expCurrentLabel->setStyleSheet("QLabel { color : green; }");

	log = new FishLogger(_MODE_TRAIN02, ratID[_MODE_TRAIN02]);
	int tri_t = trials[_MODE_TRAIN02];
	int iti_t = iti[_MODE_TRAIN02];
	log->printTitle(tri_t, iti_t, -1);
	emit StartFish_T2(tri_t, iti_t, ardPort, ardBaud);

	logger->clear();
	timerLCD->display("00:00:00");
	time.setHMS(0, 0, 0);
	simulateTime->start(1000);
}

void FishingHole::experimentSTART_T3()
{
	if (!isArdConnected()){
		return;
	}
	if(myFishExp.exp_running){
		QMessageBox *error = new QMessageBox();
		error->setText("Experiment already running\nPlease stop current experiment before\nstarting a new one");
		error->setWindowTitle("ERROR");
		error->show();
		return;
	}	
	expStatusLabel->setText("Running");
	expCurrentLabel->setText("Training P3");
	expStatusLabel->setStyleSheet("QLabel { color : green; }");
	expCurrentLabel->setStyleSheet("QLabel { color : green; }");

	log = new FishLogger(_MODE_TRAIN03, ratID[_MODE_TRAIN03]);
	int tri_t = trials[_MODE_TRAIN03];
	int iti_t = iti[_MODE_TRAIN03];
	log->printTitle(tri_t, iti_t, -1);
	emit StartFish_T3(tri_t, iti_t, ardPort, ardBaud);

	logger->clear();
	timerLCD->display("00:00:00");
	time.setHMS(0, 0, 0);
	simulateTime->start(1000);
}

void FishingHole::experimentSTART_E1()
{
	if (!isArdConnected()){
		return;
	}
	if(myFishExp.exp_running){
		QMessageBox *error = new QMessageBox();
		error->setText("Experiment already running\nPlease stop current experiment before\nstarting a new one");
		error->setWindowTitle("ERROR");
		error->show();
		return;
	}
	expStatusLabel->setText("Running");
	expCurrentLabel->setText("Experiment");
	expStatusLabel->setStyleSheet("QLabel { color : green; }");
	expCurrentLabel->setStyleSheet("QLabel { color : green; }");

	log = new FishLogger(_MODE_EXPER01, ratID[_MODE_EXPER01]);
	int tri_t = trials[_MODE_EXPER01];
	int iti_t = iti[_MODE_EXPER01];
	log->printTitle(tri_t, iti_t, pMode);
	emit StartFish_Exp(tri_t, iti_t, pMode, ardPort, ardBaud);

	logger->clear();
	timerLCD->display("00:00:00");
	time.setHMS(0, 0, 0);
	simulateTime->start(1000);
}



void FishingHole::requestExperimentEND() {
	if(!myFishExp.exp_running){
		QMessageBox *error = new QMessageBox();
		error->setText("No experiment running");
		error->setWindowTitle("ERROR");
		error->show();
		return;
	}
	myFishExp.exp_running = false;
}

void FishingHole::experimentEND()
{
	expStatusLabel->setText("Stopped");
	expCurrentLabel->setText("N/A");
	expStatusLabel->setStyleSheet("QLabel { color : red; }");
	expCurrentLabel->setStyleSheet("QLabel { color : red; }");
	simulateTime->stop();
}

void FishingHole::addLog(QString txt, int qLog)
{
	string toLog = txt.toStdString();
	log->log(toLog, qLog);

	logger->moveCursor(QTextCursor::End);
	logger->append(txt);
}

void FishingHole::clearLog()
{
	logger->clear();
}

void FishingHole::tickTock()
{
	time = time.addMSecs(1000);
	QString text = time.toString("hh:mm:ss");
	timerLCD->display(text);
}

void FishingHole::closeEvent(QCloseEvent *event)
{
	if(myFishExp.exp_running)
	{
		QMessageBox *error = new QMessageBox();
		error->setText("Experiment currently running\nPLease end the experiment before closing");
		error->setWindowTitle("ERROR");
		error->show();
		return;
	}
	
	int resBtn = QMessageBox::question(this, "Close Confirmation",
									"Are you sure you want to exit?\n",
									QMessageBox::Yes,
									QMessageBox::No);

	if(resBtn == QMessageBox::Yes){
		workerThread.quit();
		workerThread.wait();
		event->accept();
	}
	else if(resBtn == QMessageBox::No){
		event->ignore();
	}
}