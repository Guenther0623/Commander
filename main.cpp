#include <QApplication>
//#include <QtWidgets>

#include "mainwindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	
	MainWindow mainWindow;
	mainWindow.show();

	/*QPushButton button;
	QStateMachine machine;

	QState *off = new QState();
	off->assignProperty(&button, "text", "Off");
	off->setObjectName("Off");

	QState *on = new QState();
	on->assignProperty(&button, "text", "On");
	on->setObjectName("On");

	off->addTransition(&button, SIGNAL(clicked()), on);
	on->addTransition(&button, SIGNAL(clicked()), off);

	machine.addState(off);
	machine.addState(on);

	machine.setInitialState(off);
	machine.start();

	button.resize(100, 50);
	button.show();

	QPushButton *toggleButton = new QPushButton("Toggle");
	toggleButton->setCheckable(true);
	toggleButton->show();*/

	return app.exec();
}