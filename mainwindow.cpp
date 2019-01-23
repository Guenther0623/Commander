//#include <QtWidgets>
#include <iostream>
#include <QBoxLayout>
#include <QLCDNumber>

#include "mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
{
	QGridLayout *mainGrid = new QGridLayout;

	forceClosing = false;

	mainGrid->addWidget(createTitle(), 0, 0);
	mainGrid->addWidget(createInstructions(), 1, 0);
	mainGrid->addWidget(createExpButtons(), 2, 0);

	setLayout(mainGrid);
}

MainWindow::~MainWindow()
{

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

	QLabel *title = new QLabel("Commander V0.9A");
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
 *  createTitle
 *
 *  Creates the box with instructions
 *  to start the experiment of choice
 *
 */
QGroupBox *MainWindow::createInstructions()
{
	QGroupBox *groupBox = new QGroupBox();
	groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {background-color: lightgray; border-style: solid; border-width: 3px; border-radius: 10px; border-color: gray; margin-top: 1ex; font-size: 20px; font-weight: bold} QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 10px}"));

	QVBoxLayout *vbox = new QVBoxLayout;

	QLabel *label01 = new QLabel("To Start:");
	QFont labelFont("Arial", 16, QFont::Bold);	
	label01->setFont(labelFont);
	label01->setAlignment(Qt::AlignLeft);

	QLabel *label02 = new QLabel("Select an experiment below and press start to begin");

	QHBoxLayout *hbox01 = new QHBoxLayout;
	QHBoxLayout *hbox02 = new QHBoxLayout;

	hbox01->addWidget(label01);
	hbox01->addStretch(100);
	hbox02->addStretch(100);
	hbox02->addWidget(label02);
	hbox02->addStretch(100);

	vbox->addLayout(hbox01);
	vbox->addLayout(hbox02);

	groupBox->setLayout(vbox);

	return groupBox;
}

/*
 *  createExpButtons
 *
 *  Creates the box containing a button
 *  selection for each experiment 
 *
 */
QGroupBox *MainWindow::createExpButtons()
{
	QGroupBox *groupBox = new QGroupBox();

	expOptions = new QButtonGroup();
	expOptions->setExclusive(true);

	QHBoxLayout *hboxR1 = new QHBoxLayout;
	QHBoxLayout *hboxR2 = new QHBoxLayout;

	QVBoxLayout *vbox = new QVBoxLayout;

	QPushButton *expDelD = new QPushButton(tr("Delay\nDiscounting"));
	expDelD->setFixedSize(150, 40);
	expDelD->setCheckable(true);
	expOptions->addButton(expDelD, 1);

	QPushButton *expDNMS = new QPushButton(tr("Spatial\nDelay NMS"));
	expDNMS->setFixedSize(150, 40);
	expDNMS->setCheckable(true);
	expOptions->addButton(expDNMS, 2);

	QPushButton *expCirc = new QPushButton(tr("Circular\nMemory Task"));
	expCirc->setFixedSize(150, 40);
	expCirc->setCheckable(true);
	expOptions->addButton(expCirc, 3);

	QPushButton *expRand = new QPushButton(tr("Random\nOperant Task"));
	expRand->setFixedSize(150, 40);
	expRand->setCheckable(true);
	expOptions->addButton(expRand, 4);

	QPushButton *expFish = new QPushButton(tr("Fishing Hole\nOperant Task"));
	expFish->setFixedSize(150, 40);
	expFish->setCheckable(true);
	expOptions->addButton(expFish, 5);

	QPushButton *expHold = new QPushButton(tr("HOLDER\n(n.y.i.)"));
	expHold->setFixedSize(150, 40);
	expHold->setCheckable(true);
	expOptions->addButton(expHold, 6);

	hboxR1->addStretch(100);
	hboxR1->addWidget(expDelD);
	hboxR1->addSpacerItem(new QSpacerItem(10, 1));
	hboxR1->addWidget(expDNMS);
	hboxR1->addSpacerItem(new QSpacerItem(10, 1));
	hboxR1->addWidget(expCirc);
	hboxR1->addStretch(100);

	hboxR2->addStretch(100);
	hboxR2->addWidget(expRand);
	hboxR2->addSpacerItem(new QSpacerItem(10, 1));
	hboxR2->addWidget(expFish);
	hboxR2->addSpacerItem(new QSpacerItem(10, 1));
	hboxR2->addWidget(expHold);
	hboxR2->addStretch(100);	

	vbox->addSpacerItem(new QSpacerItem(1, 10));
	vbox->addStretch(100);
	vbox->addLayout(hboxR1);
	vbox->addSpacerItem(new QSpacerItem(1, 10));
	vbox->addLayout(hboxR2);
	vbox->addStretch(100);
	vbox->addSpacerItem(new QSpacerItem(1, 10));

	QPushButton *buttonSt = new QPushButton(tr("Start"));
	QPushButton *buttonEx = new QPushButton(tr("Exit"));
	QFont fontState("Arial", 13, QFont::Bold);
	buttonSt->setFixedSize(200, 50);
	buttonSt->setFont(fontState);
	buttonEx->setFixedSize(200, 50);
	buttonEx->setFont(fontState);
	connect(buttonSt, SIGNAL(released()), this, SLOT(openWindow()));
	connect(buttonEx, SIGNAL(released()), this, SLOT(closeWindow()));

	QHBoxLayout *hboxS = new QHBoxLayout;
	hboxS->addStretch(100);
	hboxS->addWidget(buttonSt);
	hboxS->addSpacerItem(new QSpacerItem(10, 1));
	hboxS->addWidget(buttonEx);
	hboxS->addStretch(100);

	vbox->addStretch(100);
	vbox->addSpacerItem(new QSpacerItem(1, 10));
	vbox->addLayout(hboxS);
	vbox->addSpacerItem(new QSpacerItem(1, 10));
	vbox->addStretch(100);

	groupBox->setLayout(vbox);

	return groupBox;
}

void MainWindow::openWindow()
{
	int selected = expOptions->checkedId();
	switch(selected){
		case -1:
		{	QMessageBox *error = new QMessageBox();
			error->setText("No experiment selected\nPlease select an experiment and try again");
			error->setWindowTitle("ERROR");
			error->show();
			break;
		}
		
		case 1:
		{	//delD.show();
			forceClosing = true;
			this->close();
		}	break;

		case 2:
		{	//dnms.show();
			forceClosing = true;
			this->close();
		}	break;

		case 3:
		{	circ.show();
			forceClosing = true;
			this->close();
		}	break;

		case 4:
		{	//rand.show();
			forceClosing = true;
			this->close();
		}	break;

		case 5:
		{	fish.show();
			forceClosing = true;
			this->close();
		}	break;

		case 6:
		{	//hold.show();
			forceClosing = true;
			this->close();
		}	break;
	}

}

void MainWindow::closeWindow()
{
	this ->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (forceClosing)
	{
		event->accept();
		return;
	}

	int resBtn = QMessageBox::question(this, "Close Confirmation",
									"Are you sure you want to exit?\n",
									QMessageBox::Yes,
									QMessageBox::No);

	if(resBtn == QMessageBox::Yes)
		event->accept();
	else if(resBtn == QMessageBox::No)
		event->ignore();
}