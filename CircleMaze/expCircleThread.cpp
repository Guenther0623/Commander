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


//namespace expCircleThread {

/*
 *	======== startCircle Experiment========
 *
 */
void expCircleThread::startCircle_Exp(int trials, int startZone, int zones, int port, int baud)
{
	
	return;
}



// /**
//  *-----------------------------------------------------------
//  *------------------Functions for Arduino--------------------
//  *-----------------------------------------------------------
//  *-----------------------------------------------------------
//  */


// /*
//  *	======== play_Sound ========
//  *
//  *  param status 
//  */
// void play_Sound(int duration)
// {
// 	bool reading = true;
// 	stringstream sstm;
// 	int soundDur = duration;
// 	string comm_st = "play_sound ";
// 	string expected = "Playing sound";
// 	sstm<<comm_st<<soundDur;

// 	string new_input = sstm.str();

// 	ard->sendCommand(new_input);


// 	while(reading){
// 		if(ard->readCommand()){
// 			QString qstr = QString::fromStdString(ard->lastLine);

// 			if(isDis){
// 				if(ard->lastComm.compare(_disabled1) != 0){
// 					//emit sendComm(qstr, 1);
// 				}
// 			}
// 			else{
// 				//emit sendComm(qstr, 1);
// 			}

// 			cout << ard->lastComm << endl;
// 			if(ard->lastComm.compare(expected) == 0){
// 				reading = false;
// 			}
// 		}
// 	}
// }

// /*
// *  LED
// *	Controls LED behavior by communicating with arduino
// *   
// *	params:
// *	  (int) pos: position of led, 0-7 or 8 for all
// *	  (int) state: state to change led to, 1 for on or 0 for off
// */
// bool led(int pos, int state) {
// 	if (pos < 0 || pos > 7) {
// 		cout << "ledOff: Invalid position" << endl;
// 		return false;
// 	}

// 	// convert position to corresponding angle
// 	int angle;
// 	for (int i = 0; i < pos; i++)
// 		angle += 45;

// 	// load command to string
// 	stringstream sstm;
// 	string comm = "LED ";
// 	sstm << comm << state << angle;
// 	string new_input = sstrm.str();

// 	// load expected results to string
// 	string stateString = (state == 1? "On" : "Off");
// 	string expected = "Turning "+ stateString + " LED 0" + angle + " degree, ";
// 	if (pos == 8) // turn all on/off
// 		expected = "Turning "+ stateString + " All LEDs, ";

// 	// send command
// 	ard->sendCommand(new_input);

// 	// read arduino comm
// 	while(reading) {
// 		if(ard->readCommand())
// 			cout << ard->lastComm << endl;
// 			if (ard->lastComm.compare(expected) == 0){
// 				reading = false;
// 			else
// 				return false;
			
// 			QString qstr = QString::fromStdString(ard->lastLine);
// 			emit sendComm(qstr, 1);
// 		}
// 	}
// }


// bool ledOn(int pos) {
// 	return led(pos, 1);
// }


// bool ledOff(int pos) {
// 	return led(pos, 0);
// }


// void clearBuffer()
// {
// 	bool clearing = true;

// 	while(clearing){
// 		if(!ard->readCommand())
// 			clearing = false;
// 	}
// }

// /*
//  * ========== End Functions for Arduino  =====================
//  */

// } // end namespace
