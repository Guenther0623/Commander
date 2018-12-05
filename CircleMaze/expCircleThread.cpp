//#include <SerialStream.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>
#include <unistd.h>
#include <vector>

#include "arduino.h"
#include "defines.h"
#include "expCircleThread.h"


/*
 *	======== startCircle Experiment========
 *
 */
void expCircleThread::startCircle_Exp(int trials, int startZone, int zones, int port, int baud)
{
	
	return;
}



/**
 *-----------------------------------------------------------
 *------------------Functions for Arduino--------------------
 *-----------------------------------------------------------
 *-----------------------------------------------------------
 */


/*
 *	======== playSound ========
 *
 *  params:
 *	  (int) pos: position of dispenser, 0-7
 */
bool expCircleThread::dispense(int pos)
{
	bool dispensed = true;
	if (pos < 0 || pos > 7) {
		cout << "dispense: Invalid position" << endl;
		return false;
	}

	// convert position to corresponding angle
	int angle;
	for (int i = 0; i < pos; i++)
		angle += 45;

	// load command to string
	stringstream sstrm;
	string comm = "REWARD ";
	sstrm << comm << angle;
	string new_input = sstrm.str();

	// load expected results to string
	string small = (angle < 100)? "0" : "";
	string expected = "Dispensing Reward " + small + std::to_string(angle) + " degree, ";

	// send command
	ard->sendCommand(new_input);

	// read arduino comm
	bool reading = true;
	while(reading) {
		if(ard->readCommand()){
			cout << ard->lastComm << endl;
			if (ard->lastComm.compare(expected) == 0)
				reading = false;
			else
				dispensed = false;
			
			QString qstr = QString::fromStdString(ard->lastLine);
			emit sendComm(qstr, 1);
		}
	}

	return dispensed;
}


/*
 *	======== playSound ========
 *
 */
bool expCircleThread::playSound()
{
	bool played = true;

	// load command to string
	stringstream sstrm;
	string comm = "TONE";

	// load expected results to string
	string expected = "Playing Sound, ";

	// send command
	ard->sendCommand(comm);

	// read arduino comm
	bool reading = true;
	while(reading) {
		if(ard->readCommand()){
			cout << ard->lastComm << endl;
			if (ard->lastComm.compare(expected) == 0)
				reading = false;
			else
				played = false;
			
			QString qstr = QString::fromStdString(ard->lastLine);
			emit sendComm(qstr, 1);
		}
	}

	return played;
}


/*
*  ======= led =======
*	Controls LED behavior
*   
*	params:
*	  (int) pos: position of led, 0-7 or 8 for all
*	  (int) state: state to change led to, 1 for on or 0 for off
*/
bool expCircleThread::led(int pos, int state)
{
	bool lit = true;

	if (pos < 0 || pos > 8) {
		cout << "led: Invalid position" << endl;
		return false;
	}

	// convert position to corresponding angle
	int angle;
	for (int i = 0; i < pos; i++)
		angle += 45;

	// load command to string
	stringstream sstrm;
	string comm = "LED ";
	sstrm << comm << state << angle;
	string new_input = sstrm.str();

	// load expected results to string
	string stateString = (state == 1? "On" : "Off");
	string small = (angle < 100)? "0" : "";
	string expected = "Turning "+ stateString + " LED " + small + std::to_string(angle) + " degree, ";
	if (pos == 8) // turn all on/off
		expected = "Turning "+ stateString + " All LEDs, ";

	// send command
	ard->sendCommand(new_input);

	// read arduino comm
	bool reading = true;
	while(reading) {
		if(ard->readCommand()) {
			cout << ard->lastComm << endl;
			if (ard->lastComm.compare(expected) == 0)
				reading = false;
			else
				lit = false;
			
			QString qstr = QString::fromStdString(ard->lastLine);
			emit sendComm(qstr, 1);
		}
	}

	return lit;
}


bool expCircleThread::ledOn(int pos) {
	return led(pos, 1);
}


bool expCircleThread::ledOff(int pos) {
	return led(pos, 0);
}


void expCircleThread::clearBuffer()
{
	bool clearing = true;

	while(clearing)
		if(!ard->readCommand())
			clearing = false;

}

/*
 * ========== End Functions for Arduino  =====================
 */
