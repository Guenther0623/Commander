//#ifndef CIRCLEMAZE_CPP
//#define CIRCLEMAZE_CPP

#include "circleMaze.h"

/*
 * CircleMaze
 *
 * Controls the layout of window with a QGridLayout
 *
 */
CircleMaze::CircleMaze(QWidget *parent)
    : QWidget(parent)
{

    mainGrid = new QGridLayout;

    time = QTime(0, 0, 0);

    ardPort = _PORT_ACM0;
    ardBaud = _BAUD_9600;
    ardTestState = _COMM_UNTESTED;

    mainGrid->addWidget(createTitle(), 0, 0, 1, 0);
    mainGrid->addWidget(createOptions(), 1, 0, 1, 0);
    mainGrid->addWidget(createExpParams(), 2, 0);
    mainGrid->addWidget(createLogger(), 2, 1);

    connect(&myCircleExp, SIGNAL(sendComm(QString, int)),
        this, SLOT(addLog(QString, int)));

    connect(&myCircleExp, SIGNAL(naturalEnd()),
            this, SLOT(experimentEND()));

    connect(this, SIGNAL(StartCircle_Exp(int, int, int*, int, int)), 
            &myCircleExp, SLOT(startCircle_Exp(int, int, int*, int, int)));

    connect(this, SIGNAL(StartCircle_T1(int, int, int, int, int)), 
            &myCircleExp, SLOT(startCircle_Tr1(int, int, int, int, int)));

    connect(this, SIGNAL(StartCircle_T2(int, int, int, int, int)), 
            &myCircleExp, SLOT(startCircle_Tr2(int, int, int, int, int)));

    setLayout(mainGrid);
    setWindowTitle(tr("Commander - Circle Maze"));

    myCircleExp.exp_running = false;
    myCircleExp.moveToThread(&workerThread);
    workerThread.start();
}

CircleMaze::~CircleMaze() 
{

}


/*
 *  createTitle
 *
 *  Creates the title box
 *
 */
QGroupBox *CircleMaze::createTitle()
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
 *  Creates the options groupbox with arduino test area
 *
 */
QGroupBox *CircleMaze::createOptions()
{
    QGroupBox *groupBox = new QGroupBox(tr("Option Select Menu"));
    groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 20px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px}"));

    QVBoxLayout *vbox = new QVBoxLayout;
    QHBoxLayout *hbox = new QHBoxLayout;

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
    connect(buttonBaud, SIGNAL(activated(int)), this, SLOT(ardBaudChanged(int)));

    /*
    QLabel *connectLabel = new QLabel("Arduino Status:");
    connectLabel->setAlignment(Qt::AlignCenter);

    ardStatusLabel = new QLabel("Untested");
    QFont font("Arial", 16, QFont::Bold);
    ardStatusLabel->setFont(font);
    ardStatusLabel->setFixedWidth(200);
    ardStatusLabel->setStyleSheet("QLabel { color : orange; }");
    ardStatusLabel->setAlignment(Qt::AlignCenter);

    QPushButton *buttonTest = new QPushButton(tr("Test\nConnection"));
    buttonTest->setMinimumWidth(100);
    buttonTest->setMaximumWidth(100);
    buttonTest->setMinimumHeight(50);
    buttonTest->setMaximumHeight(50);
    connect(buttonTest, SIGNAL(released()), this, SLOT(testArd()));

    QGridLayout *optionsGrid = new QGridLayout();
    optionsGrid->addWidget(buttonTest, 0, 0, 2, 2, Qt::AlignCenter);
    optionsGrid->addWidget(connectLabel, 0, 2, 1, 1, Qt::AlignBottom);
    optionsGrid->addWidget(ardStatusLabel, 1, 2, 1, 1, Qt::AlignTop);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(groupBoxArd);
    hbox->addSpacerItem(new QSpacerItem(60, 30)); 
    hbox->addLayout(optionsGrid);
    hbox->setAlignment(Qt::AlignHCenter);

    groupBox->setLayout(hbox);
    return groupBox;
    */

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
    buttonExp->addItem(tr("Circle Maze"));
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

    hlay_sub1->addStretch(100);     //SpacerItem(new QSpacerItem(75, 10));
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
 *  createExpParams
 *
 *  Creates the experiment parameters box
 *
 */
QGroupBox *CircleMaze::createExpParams()
{
    QGroupBox *groupBox = new QGroupBox(tr("Experiments"));
    groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 18px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px}"));

    //QGroupBox *exp_train1 = new QGroupBox;

    QTabWidget *paramTabs = new QTabWidget;
    paramTabs->addTab(createExperiment(), "Experiment");
    paramTabs->addTab(createTraining1(), "Training P1");
    paramTabs->addTab(createTraining2(), "Training P2");
    //paramTabs->addTab(createTraining3(), "Training P3");

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(paramTabs);

    groupBox->setLayout(hbox);
    return groupBox;

    /*
    QGroupBox *expParamsBox = new QGroupBox(tr("Experiment Parameters"));
    expParamsBox->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 18px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px}"));

    QSpinBox *startEdit = new QSpinBox(this);
    startEdit->setRange(1, 8);
    startEdit->setAlignment(Qt::AlignRight);
    startEdit->setFixedSize(100, 25);
    connect(startEdit, SIGNAL(valueChanged(int)), this, SLOT(updateValue_startZone(int)));
    QLineEdit *zoneEdit = new QLineEdit(this);
    zoneEdit->setValidator(new QIntValidator(zoneEdit));
    zoneEdit->setAlignment(Qt::AlignRight);
    zoneEdit->setFixedSize(100, 25);
    connect(zoneEdit, SIGNAL(textChanged(QString)), this, SLOT(updateValue_zones(QString)));
    QLineEdit *trialEdit = new QLineEdit(this);
    trialEdit->setValidator(new QIntValidator(trialEdit));
    trialEdit->setAlignment(Qt::AlignRight);
    trialEdit->setFixedSize(100, 25);
    connect(trialEdit, SIGNAL(textChanged(QString)), this, SLOT(updateValue_trials(QString)));

    QFormLayout *paramForm = new QFormLayout;
    paramForm->setFormAlignment(Qt::AlignCenter);
    paramForm->setSpacing(15);
    paramForm->addRow("Start zone", startEdit);
    paramForm->addRow("Total zones", zoneEdit);
    paramForm->addRow("Total trials", trialEdit);

    QPushButton *buttonStrt = new QPushButton("Start Recording");
    QPushButton *buttonStop = new QPushButton("Stop Recording");
    connect(buttonStrt, SIGNAL(released()), this, SLOT(experimentSTART()));
    connect(buttonStop, SIGNAL(released()), this, SLOT(experimentEND()));

    buttonStrt->setMinimumWidth(180);
    buttonStrt->setMaximumWidth(180);
    buttonStop->setMinimumWidth(180);
    buttonStop->setMaximumWidth(180);
    buttonStrt->setMinimumHeight(50);
    buttonStrt->setMaximumHeight(50);
    buttonStop->setMinimumHeight(50);
    buttonStop->setMaximumHeight(50);

    QHBoxLayout *hboxStart = new QHBoxLayout;
    hboxStart->addSpacerItem(new QSpacerItem(60, 30));
    hboxStart->addWidget(buttonStrt);
    hboxStart->addSpacerItem(new QSpacerItem(60, 30));

    QHBoxLayout *hboxStop = new QHBoxLayout;
    hboxStop->addSpacerItem(new QSpacerItem(60, 30));
    hboxStop->addWidget(buttonStop);
    hboxStop->addSpacerItem(new QSpacerItem(60, 30));

    QVBoxLayout *vbox = new QVBoxLayout;

    vbox->addSpacerItem(new QSpacerItem(20,10));
    vbox->addLayout(paramForm);
    vbox->addLayout(hboxStart);
    vbox->addSpacerItem(new QSpacerItem(20, 10));
    vbox->addLayout(hboxStop);
    vbox->addSpacerItem(new QSpacerItem(20, 50));

    expParamsBox->setLayout(vbox);

    return expParamsBox;
    */
}


QGroupBox *CircleMaze::createExperiment()
{
    for(int i = 0; i < 8; i++)
        Circle_e_zones[i] = -1;

    Circle_e_delNum = 1;

    QGroupBox *groupBox = new QGroupBox;

    QLabel *labelRat = new QLabel("Rat ID");
    QLineEdit *lineRat = new QLineEdit();
    lineRat->setFixedSize(100, 25);
    lineRat->setAlignment(Qt::AlignRight);

    QHBoxLayout *hboxRat = new QHBoxLayout;
    hboxRat->addWidget(labelRat);
    hboxRat->addWidget(lineRat);

    QLabel *labelSZone = new QLabel("Start Zone");
    QLineEdit *lineSZone = new QLineEdit();
    lineSZone->setFixedSize(100, 25);
    lineSZone->setValidator(new QIntValidator(lineSZone));
    lineSZone->setAlignment(Qt::AlignRight);

    QHBoxLayout *hboxSZone = new QHBoxLayout;
    hboxSZone->addWidget(labelSZone);
    hboxSZone->addWidget(lineSZone);

    QSpinBox *delNumSpinBox = new QSpinBox;
    delNumSpinBox->setRange(1, 8);
    delNumSpinBox->setSingleStep(1);
    delNumSpinBox->setValue(1);
    delNumSpinBox->setFixedSize(100, 25);
    connect(delNumSpinBox, SIGNAL(valueChanged(int)), this, SLOT(delaySpinChanged(int)));
    QLabel *labelSpin = new QLabel("Number of Zones");

    QHBoxLayout *hboxDelN = new QHBoxLayout;
    hboxDelN->addWidget(labelSpin);
    hboxDelN->addWidget(delNumSpinBox);

    QGroupBox *grpPatternMode = new QGroupBox(tr("Pattern Select"));
    grpPatternMode->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 15px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 3px}"));

    QLabel *labelDel1 = new QLabel("D1: ");
    QLabel *labelDel2 = new QLabel("D2: ");
    QLabel *labelDel3 = new QLabel("D3: ");
    QLabel *labelDel4 = new QLabel("D4: ");
    QVBoxLayout *vboxDel1Labels = new QVBoxLayout();
    vboxDel1Labels->addWidget(labelDel1);
    vboxDel1Labels->addWidget(labelDel2);
    vboxDel1Labels->addWidget(labelDel3);
    vboxDel1Labels->addWidget(labelDel4);

    QLabel *labelDel5 = new QLabel("D5: ");
    QLabel *labelDel6 = new QLabel("D6: ");
    QLabel *labelDel7 = new QLabel("D7: ");
    QLabel *labelDel8 = new QLabel("D8: ");
    QVBoxLayout *vboxDel2Labels = new QVBoxLayout();
    vboxDel2Labels->addWidget(labelDel5);
    vboxDel2Labels->addWidget(labelDel6);
    vboxDel2Labels->addWidget(labelDel7);
    vboxDel2Labels->addWidget(labelDel8);

    lineDel1 = new QLineEdit();
    lineDel1->setValidator(new QIntValidator(lineDel1));
    lineDel1->setDisabled(false);
    lineDel1->setFixedSize(100, 25);
    lineDel2 = new QLineEdit();
    lineDel2->setValidator(new QIntValidator(lineDel2));
    lineDel2->setDisabled(true);
    lineDel2->setFixedSize(100, 25);
    lineDel3 = new QLineEdit();
    lineDel3->setValidator(new QIntValidator(lineDel3));
    lineDel3->setDisabled(true);
    lineDel3->setFixedSize(100, 25);
    lineDel4 = new QLineEdit();
    lineDel4->setValidator(new QIntValidator(lineDel4));
    lineDel4->setDisabled(true);
    lineDel4->setFixedSize(100, 25);
    lineDel5 = new QLineEdit();
    lineDel5->setValidator(new QIntValidator(lineDel5));
    lineDel5->setDisabled(true);
    lineDel5->setFixedSize(100, 25);
    lineDel6 = new QLineEdit();
    lineDel6->setValidator(new QIntValidator(lineDel6));
    lineDel6->setDisabled(true);
    lineDel6->setFixedSize(100, 25);
    lineDel7 = new QLineEdit();
    lineDel7->setValidator(new QIntValidator(lineDel7));
    lineDel7->setDisabled(true);
    lineDel7->setFixedSize(100, 25);
    lineDel8 = new QLineEdit();
    lineDel8->setValidator(new QIntValidator(lineDel8));
    lineDel8->setDisabled(true);
    lineDel8->setFixedSize(100, 25);

    connect(lineDel1, SIGNAL(textChanged(QString)), this, SLOT(circleL1Changed(QString)));
    connect(lineDel2, SIGNAL(textChanged(QString)), this, SLOT(circleL2Changed(QString)));
    connect(lineDel3, SIGNAL(textChanged(QString)), this, SLOT(circleL3Changed(QString)));
    connect(lineDel4, SIGNAL(textChanged(QString)), this, SLOT(circleL4Changed(QString)));
    connect(lineDel5, SIGNAL(textChanged(QString)), this, SLOT(circleL5Changed(QString)));
    connect(lineDel6, SIGNAL(textChanged(QString)), this, SLOT(circleL6Changed(QString)));
    connect(lineDel7, SIGNAL(textChanged(QString)), this, SLOT(circleL7Changed(QString)));
    connect(lineDel8, SIGNAL(textChanged(QString)), this, SLOT(circleL8Changed(QString)));

    QVBoxLayout *vboxDel1Lines = new QVBoxLayout();
    vboxDel1Lines->addWidget(lineDel1);
    vboxDel1Lines->addWidget(lineDel2);
    vboxDel1Lines->addWidget(lineDel3);
    vboxDel1Lines->addWidget(lineDel4);
    QVBoxLayout *vboxDel2Lines = new QVBoxLayout();
    vboxDel2Lines->addWidget(lineDel5);
    vboxDel2Lines->addWidget(lineDel6);
    vboxDel2Lines->addWidget(lineDel7);
    vboxDel2Lines->addWidget(lineDel8); 

    QHBoxLayout *hboxDelMenu = new QHBoxLayout();
    hboxDelMenu->addSpacerItem(new QSpacerItem(10, 10));
    hboxDelMenu->addLayout(vboxDel1Labels);
    hboxDelMenu->addLayout(vboxDel1Lines);
    hboxDelMenu->addSpacerItem(new QSpacerItem(40, 40));
    hboxDelMenu->addLayout(vboxDel2Labels);
    hboxDelMenu->addLayout(vboxDel2Lines);
    hboxDelMenu->addSpacerItem(new QSpacerItem(10, 10));

    grpPatternMode->setLayout(hboxDelMenu);

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
    vbox->addLayout(hboxSZone);
    vbox->addSpacerItem(new QSpacerItem(10, 8));
    vbox->addLayout(hboxDelN);
    vbox->addSpacerItem(new QSpacerItem(10, 8));
    vbox->addWidget(grpPatternMode);
    vbox->addSpacerItem(new QSpacerItem(10, 8));
    vbox->addLayout(hboxStart);
    vbox->addSpacerItem(new QSpacerItem(20, 10));
    vbox->addLayout(hboxStop);
    vbox->addSpacerItem(new QSpacerItem(20, 10));

    connect(lineRat, SIGNAL(textChanged(QString)),
            this, SLOT(ratChanged_E1(QString)));
    connect(lineSZone, SIGNAL(textChanged(QString)),
            this, SLOT(sZoneChanged_E1(QString)));
    connect(buttonStrt, SIGNAL(released()),
            this, SLOT(experimentSTART_E1()));
    connect(buttonStop, SIGNAL(released()), 
            this, SLOT(requestExperimentEND()));

    groupBox->setLayout(vbox);

    return groupBox;
}


QGroupBox *CircleMaze::createTraining1()
{
    QGroupBox *groupBox = new QGroupBox;

    QLineEdit *ratEdit = new QLineEdit(this);
    ratEdit->setAlignment(Qt::AlignRight);
    ratEdit->setFixedSize(100, 25);
    connect(ratEdit, SIGNAL(textChanged(QString)), this, SLOT(ratChanged_TP1(QString)));

    QLineEdit *trialEdit = new QLineEdit(this);
    trialEdit->setValidator(new QIntValidator(trialEdit));
    trialEdit->setAlignment(Qt::AlignRight);
    trialEdit->setFixedSize(100, 25);
    connect(trialEdit, SIGNAL(textChanged(QString)), this, SLOT(trialsChanged_TP1(QString)));

    QLineEdit *ITIEdit = new QLineEdit(this);
    ITIEdit->setValidator(new QIntValidator(ITIEdit));
    ITIEdit->setAlignment(Qt::AlignRight);
    ITIEdit->setFixedSize(100, 25);
    connect(ITIEdit, SIGNAL(textChanged(QString)), this, SLOT(sITIChanged_TP1(QString)));
    
    QLineEdit *startEdit = new QLineEdit(this);
    startEdit->setValidator(new QIntValidator(startEdit));
    startEdit->setAlignment(Qt::AlignRight);
    startEdit->setFixedSize(100, 25);
    connect(startEdit, SIGNAL(textChanged(QString)), this, SLOT(sZoneChanged_TP1(QString)));

    QFormLayout *paramForm = new QFormLayout;
    paramForm->setFormAlignment(Qt::AlignCenter);
    paramForm->setVerticalSpacing(15);
    paramForm->setHorizontalSpacing(50);
    paramForm->addRow("Rat ID", ratEdit);
    paramForm->addRow("Trial Count", trialEdit);
    paramForm->addRow("ITI", ITIEdit);
    paramForm->addRow("Start zone", startEdit);

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
    vbox->addStretch(10);
    vbox->addLayout(paramForm);
    vbox->addStretch(20);
    vbox->addLayout(hboxStart);
    vbox->addSpacerItem(new QSpacerItem(20, 10));
    vbox->addLayout(hboxStop);
    vbox->addSpacerItem(new QSpacerItem(20, 10));

    connect(buttonStrt, SIGNAL(released()),
            this, SLOT(experimentSTART_T1()));
    connect(buttonStop, SIGNAL(released()), 
            this, SLOT(requestExperimentEND()));

    groupBox->setLayout(vbox);
    return groupBox;
}

QGroupBox *CircleMaze::createTraining2()
{
    QGroupBox *groupBox = new QGroupBox;

    QLineEdit *ratEdit = new QLineEdit(this);
    ratEdit->setAlignment(Qt::AlignRight);
    ratEdit->setFixedSize(100, 25);
    connect(ratEdit, SIGNAL(textChanged(QString)), this, SLOT(ratChanged_TP2(QString)));

    QLineEdit *trialEdit = new QLineEdit(this);
    trialEdit->setValidator(new QIntValidator(trialEdit));
    trialEdit->setAlignment(Qt::AlignRight);
    trialEdit->setFixedSize(100, 25);
    connect(trialEdit, SIGNAL(textChanged(QString)), this, SLOT(trialsChanged_TP2(QString)));

    QLineEdit *ITIEdit = new QLineEdit(this);
    ITIEdit->setValidator(new QIntValidator(ITIEdit));
    ITIEdit->setAlignment(Qt::AlignRight);
    ITIEdit->setFixedSize(100, 25);
    connect(ITIEdit, SIGNAL(textChanged(QString)), this, SLOT(sITIChanged_TP2(QString)));

    QLineEdit *startEdit = new QLineEdit(this);
    startEdit->setValidator(new QIntValidator(startEdit));
    startEdit->setAlignment(Qt::AlignRight);
    startEdit->setFixedSize(100, 25);
    connect(startEdit, SIGNAL(textChanged(QString)), this, SLOT(sZoneChanged_TP2(QString)));

    QFormLayout *paramForm = new QFormLayout;
    paramForm->setFormAlignment(Qt::AlignCenter);
    paramForm->setVerticalSpacing(15);
    paramForm->setHorizontalSpacing(50);
    paramForm->addRow("Rat ID", ratEdit);
    paramForm->addRow("Trial Count", trialEdit);
    paramForm->addRow("ITI", ITIEdit);
    paramForm->addRow("Start zone", startEdit);

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
    vbox->addStretch(10);
    vbox->addLayout(paramForm);
    vbox->addStretch(20);
    vbox->addLayout(hboxStart);
    vbox->addSpacerItem(new QSpacerItem(20, 10));
    vbox->addLayout(hboxStop);
    vbox->addSpacerItem(new QSpacerItem(20, 10));

    connect(buttonStrt, SIGNAL(released()),
            this, SLOT(experimentSTART_T2()));
    connect(buttonStop, SIGNAL(released()), 
            this, SLOT(requestExperimentEND()));

    groupBox->setLayout(vbox);
    return groupBox;
}



QGroupBox *CircleMaze::createLogger()
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
    hlay_sub1->addStretch(100);     //SpacerItem(new QSpacerItem(75, 10));
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
    hlay_sub3->addStretch(100);     //SpacerItem(new QSpacerItem(75, 10));
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


// /*
//  * checkTestState
//  *
//  * Communicates with arduino to verify connection
//  *
//  */
// bool CircleMaze::checkTestState()
// {
//     if (ardTestState == _COMM_UNTESTED)
//     {
//         QMessageBox* error = new QMessageBox();
//         error->setText("Arduino Communication not Tested\nPlease test the Arduino Communication\nand try again");
//         error->setWindowTitle("ERROR");
//         error->show();
//         return false;
//     }
//     else if (ardTestState == _COMM_WRONG || ardTestState == _COMM_MISSING)
//     {
//         QMessageBox* error = new QMessageBox();
//         error->setText("Arduino Communication test Failed\nPlease retest the Arduino Communication\nand try again");
//         error->setWindowTitle("ERROR");
//         error->show();
//         return false;
//     }
//     else
//     {
//         if (expWatcher->isRunning()){ //experiment thread running
//             QMessageBox* error = new QMessageBox();
//             error->setText("Experiment is Currently Running\nPlease end Current Experiment\nand try again");
//             error->setWindowTitle("ERROR");
//             error->show();
//             return false;
//         }
//         else{
//             return true;
//         }
//     }
// }


/*******************************
 *       Slot Definitions      *
 *******************************/

void CircleMaze::experimentSTART_E1()
{
    if (!isArdConnected()){
        return;
    }
    if(myCircleExp.exp_running){
        QMessageBox *error = new QMessageBox();
        error->setText("Experiment already running\nPlease stop current experiment before\nstarting a new one");
        error->setWindowTitle("ERROR");
        error->show();
        return;
    }

    // Verify that the number of zones matches the zoneNum value
    int cntZones = 0;
    for(int i = 0; i < 8; i++)
    {
        if(Circle_e_zones[i] != -1){
            cntZones++;
            cout << "At " << i << ": " << Circle_e_zones[i] << endl;
        }
    }
    cout << cntZones << ", " << Circle_e_delNum << endl;

    if (cntZones == Circle_e_delNum)
    {
        expStatusLabel->setText("Running");
        expCurrentLabel->setText("Experiment");
        expStatusLabel->setStyleSheet("QLabel { color : green; }");
        expCurrentLabel->setStyleSheet("QLabel { color : green; }");

        log = new CircleLogger(_MODE_EXPER01, ratID[_MODE_EXPER01]); //TODO: Insert rat id? or get rid of
        int tri_t = trials[_MODE_EXPER01];
        int startZone_t = startZone[_MODE_EXPER01];
        int zones_t = zones[_MODE_EXPER01];

        log->printTitle(startZone_t, zones_t, tri_t);
        emit StartCircle_Exp(Circle_e_delNum, startZone_t, Circle_e_zones, ardPort, ardBaud);

        logger->clear();
        timerLCD->display("00:00:00");
        time.setHMS(0, 0, 0);
        simulateTime->start(1000);
    }

    else
    {
        QMessageBox* error = new QMessageBox();
        error->setText("ERROR:\n\tNumber of entered delays does not\n\tmatch the number of delays expected");
        error->setWindowTitle("ERROR");
        error->show();  
    }

}


void CircleMaze::experimentSTART_T1()
{
    if (!isArdConnected()){
        return;
    }
    if(myCircleExp.exp_running){
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

    log = new CircleLogger(_MODE_TRAIN01, ratID[_MODE_TRAIN01]); //TODO: Insert rat id? or get rid of
    int tri_t = trials[_MODE_TRAIN01];
    int startZone_t = startZone[_MODE_TRAIN01];
    int iti_t = ITI[_MODE_TRAIN01];

    log->printTitle(startZone_t, iti_t, tri_t);
    emit StartCircle_T1(tri_t, startZone_t, iti_t, ardPort, ardBaud);

    logger->clear();
    timerLCD->display("00:00:00");
    time.setHMS(0, 0, 0);
    simulateTime->start(1000);
}

void CircleMaze::experimentSTART_T2()
{
    if (!isArdConnected()){
        return;
    }
    if(myCircleExp.exp_running){
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

    log = new CircleLogger(_MODE_TRAIN02, ratID[_MODE_TRAIN02]); //TODO: Insert rat id? or get rid of
    int tri_t = trials[_MODE_TRAIN02];
    int startZone_t = startZone[_MODE_TRAIN02];
    int iti_t = ITI[_MODE_TRAIN02];

    log->printTitle(startZone_t, iti_t, tri_t);
    emit StartCircle_T2(tri_t, startZone_t, iti_t, ardPort, ardBaud);

    logger->clear();
    timerLCD->display("00:00:00");
    time.setHMS(0, 0, 0);
    simulateTime->start(1000);
}


void CircleMaze::requestExperimentEND() {
    if(!myCircleExp.exp_running){
        QMessageBox *error = new QMessageBox();
        error->setText("No experiment running");
        error->setWindowTitle("ERROR");
        error->show();
        return;
    }
    myCircleExp.exp_running = false;
}


void CircleMaze::experimentEND(){
    expStatusLabel->setText("Stopped");
    expCurrentLabel->setText("N/A");
    expStatusLabel->setStyleSheet("QLabel { color : red; }");
    expCurrentLabel->setStyleSheet("QLabel { color : red; }");
    simulateTime->stop();
}


void CircleMaze::ardPortChanged(int index)
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

void CircleMaze::ardBaudChanged(int index)
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

void CircleMaze::testArd()
{
    int ardTest = _COMM_UNTESTED;
    Arduino* ard = new Arduino(ardPort, ardBaud);

    //find arduino state
    if(ard->ardExists)
    {
        ard->sendCommand("Iden");
        usleep(2000);
        if(ard->readCommand())
        {
            string expected = "Maurer Lab Arduino: MC";
            if(ard->lastComm.compare(expected) == 0){
                cout << ard->lastComm << endl;
                ardTest = _COMM_SUCCEEDED;
            }
            else
                ardTest = _COMM_WRONG;
        }
        else
            ardTest = _COMM_WRONG;
    }
    else
        ardTest = _COMM_MISSING;


    //if arduino state changed
    if(ardTest != ardTestState)
    {
        //set global
        ardTestState = ardTest;

        //redraw label
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


bool CircleMaze::isArdConnected() {
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


void CircleMaze::circleL1Changed(QString text)
{
    if(text.isEmpty() || text == "\b")
        Circle_e_zones[0] = -1;

    Circle_e_zones[0] = text.toInt();
}

void CircleMaze::circleL2Changed(QString text)
{
    if(text.isEmpty() || text == "\b")
        Circle_e_zones[1] = -1;

    Circle_e_zones[1] = text.toInt();
}

void CircleMaze::circleL3Changed(QString text)
{
    if(text.isEmpty() || text == "\b")
        Circle_e_zones[2] = -1;

    Circle_e_zones[2] = text.toInt();
}

void CircleMaze::circleL4Changed(QString text)
{
    if(text.isEmpty() || text == "\b")
        Circle_e_zones[3] = -1;

    Circle_e_zones[3] = text.toInt();
}

void CircleMaze::circleL5Changed(QString text)
{
    if(text.isEmpty() || text == "\b")
        Circle_e_zones[4] = -1;

    Circle_e_zones[4] = text.toInt();
}

void CircleMaze::circleL6Changed(QString text)
{
    if(text.isEmpty() || text == "\b")
        Circle_e_zones[5] = -1;

    Circle_e_zones[5] = text.toInt();
}

void CircleMaze::circleL7Changed(QString text)
{
    if(text.isEmpty() || text == "\b")
        Circle_e_zones[6] = -1;

    Circle_e_zones[6] = text.toInt();
}

void CircleMaze::circleL8Changed(QString text)
{
    if(text.isEmpty() || text == "\b")
        Circle_e_zones[7] = -1;

    Circle_e_zones[7] = text.toInt();
}


void CircleMaze::ratChanged_E1(QString text)
{   ratID[_MODE_EXPER01] = text.toStdString();  }

void CircleMaze::sZoneChanged_E1(QString text)
{   startZone[_MODE_EXPER01] = text.toInt();  }

void CircleMaze::trialsChanged_E1(QString text)
{   trials[_MODE_EXPER01] = text.toInt();  }


void CircleMaze::ratChanged_TP1(QString text)
{   ratID[_MODE_TRAIN01] = text.toStdString();  }

void CircleMaze::trialsChanged_TP1(QString text)
{   trials[_MODE_TRAIN01] = text.toInt();  }

void CircleMaze::sZoneChanged_TP1(QString text)
{   startZone[_MODE_TRAIN01] = text.toInt();  }

void CircleMaze::sITIChanged_TP1(QString text)
{   ITI[_MODE_TRAIN01] = text.toInt();  }

void CircleMaze::ratChanged_TP2(QString text)
{   ratID[_MODE_TRAIN02] = text.toStdString();  }

void CircleMaze::trialsChanged_TP2(QString text)
{   trials[_MODE_TRAIN02] = text.toInt();  }

void CircleMaze::sZoneChanged_TP2(QString text)
{   startZone[_MODE_TRAIN02] = text.toInt();  }

void CircleMaze::sITIChanged_TP2(QString text)
{   ITI[_MODE_TRAIN02] = text.toInt();  }


void CircleMaze::addLog(QString txt, int qLog)
{
    string toLog = txt.toStdString();
    log->log(toLog, qLog);

    logger->moveCursor(QTextCursor::End);
    logger->append(txt);
}

void CircleMaze::clearLog()
{
    logger->clear();
}

void CircleMaze::tickTock()
{
    time = time.addMSecs(1000);
    QString text = time.toString("hh:mm:ss");
    timerLCD->display(text);
}


void CircleMaze::closeEvent(QCloseEvent *event)
{
    if(myCircleExp.exp_running)
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


void CircleMaze::delaySpinChanged(int del)
{

    Circle_e_delNum = del;

    switch(del)
    {
        case 1:
            lineDel1->setDisabled(false);
            lineDel2->setDisabled(true);
            lineDel2->setText("");
            lineDel3->setDisabled(true);
            lineDel3->setText("");
            lineDel4->setDisabled(true);
            lineDel4->setText("");
            lineDel5->setDisabled(true);
            lineDel5->setText("");
            lineDel6->setDisabled(true);
            lineDel6->setText("");
            lineDel7->setDisabled(true);
            lineDel7->setText("");
            lineDel8->setDisabled(true);
            lineDel8->setText("");
            break;

        case 2:
            lineDel1->setDisabled(false);
            lineDel2->setDisabled(false);
            lineDel3->setDisabled(true);
            lineDel3->setText("");
            lineDel4->setDisabled(true);
            lineDel4->setText("");
            lineDel5->setDisabled(true);
            lineDel5->setText("");
            lineDel6->setDisabled(true);
            lineDel6->setText("");
            lineDel7->setDisabled(true);
            lineDel7->setText("");
            lineDel8->setDisabled(true);
            lineDel8->setText("");
            break;

        case 3:
            lineDel1->setDisabled(false);
            lineDel2->setDisabled(false);
            lineDel3->setDisabled(false);
            lineDel4->setDisabled(true);
            lineDel4->setText("");
            lineDel5->setDisabled(true);
            lineDel5->setText("");
            lineDel6->setDisabled(true);
            lineDel6->setText("");
            lineDel7->setDisabled(true);
            lineDel7->setText("");
            lineDel8->setDisabled(true);
            lineDel8->setText("");
            break;

        case 4:
            lineDel1->setDisabled(false);
            lineDel2->setDisabled(false);
            lineDel3->setDisabled(false);
            lineDel4->setDisabled(false);
            lineDel5->setDisabled(true);
            lineDel5->setText("");
            lineDel6->setDisabled(true);
            lineDel6->setText("");
            lineDel7->setDisabled(true);
            lineDel7->setText("");
            lineDel8->setDisabled(true);
            lineDel8->setText("");
            break;

        case 5:
            lineDel1->setDisabled(false);
            lineDel2->setDisabled(false);
            lineDel3->setDisabled(false);
            lineDel4->setDisabled(false);
            lineDel5->setDisabled(false);
            lineDel6->setDisabled(true);
            lineDel6->setText("");
            lineDel7->setDisabled(true);
            lineDel7->setText("");
            lineDel8->setDisabled(true);
            lineDel8->setText("");
            break;

        case 6:
            lineDel1->setDisabled(false);
            lineDel2->setDisabled(false);
            lineDel3->setDisabled(false);
            lineDel4->setDisabled(false);
            lineDel5->setDisabled(false);
            lineDel6->setDisabled(false);
            lineDel7->setDisabled(true);
            lineDel7->setText("");
            lineDel8->setDisabled(true);
            lineDel8->setText("");
            break;

        case 7:
            lineDel1->setDisabled(false);
            lineDel2->setDisabled(false);
            lineDel3->setDisabled(false);
            lineDel4->setDisabled(false);
            lineDel5->setDisabled(false);
            lineDel6->setDisabled(false);
            lineDel7->setDisabled(false);
            lineDel8->setDisabled(true);
            lineDel8->setText("");
            break;

        case 8:
            lineDel1->setDisabled(false);
            lineDel2->setDisabled(false);
            lineDel3->setDisabled(false);
            lineDel4->setDisabled(false);
            lineDel5->setDisabled(false);
            lineDel6->setDisabled(false);
            lineDel7->setDisabled(false);
            lineDel8->setDisabled(false);
            break;

        default:
            break;
    }

    for(int i = del; i < 8; i++)
        Circle_e_zones[i] = -1;
}



//#endif // CIRCLEMAZE_CPP

