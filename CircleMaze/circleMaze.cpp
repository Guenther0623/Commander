#ifndef CIRCLEMAZE_CPP
#define CIRCLEMAZE_CPP

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

    time = QTime(0, 0, 0);

    mainGrid = new QGridLayout;
    mainGrid->addWidget(createTitle(), 0, 0, 1, 0);
    mainGrid->addWidget(createOptions(), 1, 0, 1, 0);
    mainGrid->addWidget(createExpParams(), 2, 0);
    mainGrid->addWidget(createLogger(), 2, 1);
    setLayout(mainGrid);
    setWindowTitle(tr("Commander - Circle Maze"));

    connect(&myCircleExp, SIGNAL(sendComm(QString, int)),
        this, SLOT(addLog(QString, int)));

    connect(&myCircleExp, SIGNAL(naturalEnd()),
            this, SLOT(experimentEND()));

    connect(this, SIGNAL(StartCircle(int, int, int, int, int)), 
            &myCircleExp, SLOT(startCircle_Exp(int, int, int, int, int)));

    myCircleExp.exp_running = false;
    myCircleExp.moveToThread(&workerThread);
    workerThread.start();
}

CircleMaze::~CircleMaze() {

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

    QLabel *title = new QLabel("Commander");
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
    QGroupBox *groupBox = new QGroupBox(tr("Arduino"));
    groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {border: 1px solid gray; border-radius: 5px; margin-top: 1ex; font-size: 20px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 3px}"));

    QGroupBox *groupBoxArd = new QGroupBox();
    groupBoxArd->setStyleSheet(QString::fromUtf8("QGroupBox {border: 0; margin-top: 2ex; margin-left: 1ex; font-size: 18px}"));
    QLabel *labelPort = new QLabel("Port: ");
    QLabel *labelBaud = new QLabel("Baudrate: ");
    QComboBox *buttonPort = new QComboBox;
    QComboBox *buttonBaud = new QComboBox;
    buttonPort->setFixedSize(200, 30);
    buttonBaud->setFixedSize(200, 30);

    QGridLayout *gridLayoutArd = new QGridLayout();
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
}


/*
 *  createExpParams
 *
 *  Creates the experiment parameters box
 *
 */
QGroupBox *CircleMaze::createExpParams()
{
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

void CircleMaze::updateValue_startZone(int val){
    startZone = val;
}

void CircleMaze::updateValue_zones(QString text){
    zones = text.toInt();
}

void CircleMaze::updateValue_trials(QString text){
    trials = text.toInt();
}


void CircleMaze::experimentSTART(){
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

    log = new CircleLogger(_MODE_EXPER, "?"); //TODO: Insert rat id? or get rid of
    log->printTitle(startZone, zones, trials);
    emit StartCircle(startZone, zones, trials, ardPort, ardBaud);

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
            string expected = "Maurer Lab Arduino: CF";
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


#endif // CIRCLEMAZE_CPP
