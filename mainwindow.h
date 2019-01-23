#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#include "CircleMaze/circleMaze.h"
#include "FishingHole/fishinghole.h"

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	bool forceClosing;
	QButtonGroup *expOptions;

	QGroupBox *createTitle();
	QGroupBox *createInstructions();
	QGroupBox *createExpButtons();

	FishingHole fish;
	CircleMaze circ;

	void closeEvent(QCloseEvent *event);

private slots:
	void openWindow();
	void closeWindow();
};

#endif //MAINWINDOW_H