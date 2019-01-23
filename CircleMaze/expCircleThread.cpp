//#include <SerialStream.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <string>

#include "arduino.h"
#include "defines.h"
#include "expCircleThread.h"


/*
 *	======== startCircle Experiment========
 *
 */
void expCircleThread::startCircle_Exp(int zoneNum, int startZone, int* zones, int port, int baud)
{
	ard = new Arduino(port, baud);
	ard->setTimeout_us(500);


	// Two tones: one right, one wrong

	// Filling Arrays with necessary strings for comparison and degrees. IDK if this is the best way to do this (most likely not)
	// But I think the GUI stuff is working so far for the Experiment, this function has to be coded up now.
	// IF you need the GUI code from the DNMS room, it's Desktop/GUI_Commander_Update/Commander
	// THANKS ALEX! You're the best! You rock!
	bool checkZones[8] = {0,0,0,0,0,0,0,0};
	cout << "Starting Experiment" << endl;
	exp_running = true;

	int zoneDegArr[zoneNum];
	string zoneStringArr[zoneNum];

	for(int i = 0; i < zoneNum; i++)
	{
		zoneDegArr[i] = zones[i] * 45;
		if (zoneDegArr[i] < 100 && zoneDegArr[i] != 0)
			zoneStringArr[i] = "0" + to_string(zoneDegArr[i]) + " Broken";
		else if (zoneDegArr[i] == 0)
			zoneStringArr[i] = "000 Broken";
		else
			zoneStringArr[i] = to_string(zoneDegArr[i]) + " Broken";
	}

	// Debugging
	for (int i = 0; i < zoneNum; i++)
	{
		cout << zones[i] << " " << zoneDegArr[i] << " " << zoneStringArr[i] << endl;
	}

	cout << "zoneNum is: " << zoneNum << endl;
	cout << "Start Zone is: " << startZone << endl;


	emit sendComm("************Experiment Start************", 0);
	emit sendComm("****************************************\n", 0);
	emit updateWindow();

	clearBuffer();

	ledOff(8);

	int iterator = 0;

	bool mazeNoLED = false;
	bool startTrial = true;
	while(exp_running)
	{
		cout << "GET HERE" << endl;
		if (startTrial)
		{
			//First, verify rat makes it to start zone
			signAndWait(startZone, 1);
			startTrial = false;
		}

		if (!mazeNoLED)
			ledOn(zones[iterator]);
		if(ard->readCommand()){	
			if(ard->lastComm.compare(zoneStringArr[iterator]) == 0){
				QString qstr = QString::fromStdString(ard->lastLine);
				emit sendComm(qstr, 1);
				playSound(200);
				if (!mazeNoLED){
					ledOff(zones[iterator]);
					iterator++;
					ledOn(zones[iterator]);					
				}
				
				if(iterator == zoneNum){
					startTrial = true;
					mazeNoLED  = true;
					iterator = 0;
				}
			}
			else
			{
				if (!mazeNoLED){

				}
				else {
					playSound(100);
					iterator++;
					if(iterator == zoneNum){
						exp_running = false;
					}
				}
			}

		}
		
	}	
	
	return;
}

void expCircleThread::startCircle_Tr1(int trials, int zoneOne, int iti, int port, int baud)
{
	ard = new Arduino(port, baud);
	ard->setTimeout_us(500);	

	bool check_Z1 = false;
	bool check_Z2 = false;
	cout << "Starting Test One" << endl;
	exp_running = true;

	// Set up zone one and zone two with given zone
	int zoneTwo = (zoneOne+4) % 8;

	string zoneOneBroken = "";
	string zoneTwoBroken = "";

	int zoneOneDeg;
	int zoneTwoDeg;

	zoneOneDeg = zoneOne * 45;
	zoneTwoDeg = zoneTwo * 45;

	if (zoneOneDeg == 0)
		zoneOneBroken = "00" + to_string(zoneOneDeg) + " Broken";
	else if (zoneOneDeg < 100)
		zoneOneBroken = "0" + to_string(zoneOneDeg) + " Broken";
	else
		zoneOneBroken = to_string(zoneOneDeg) + " Broken";

	if (zoneTwoDeg == 0)
		zoneTwoBroken = "00" + to_string(zoneTwoDeg) + " Broken";
	else if (zoneTwoDeg < 100)
		zoneTwoBroken = "0" + to_string(zoneTwoDeg) + " Broken";
	else
		zoneTwoBroken = to_string(zoneTwoDeg) + " Broken";

	cout << "Zone one: " << zoneOneBroken << endl;
	cout << "Zone two: " << zoneTwoBroken << endl;

	emit sendComm("************Begin Training 1************", 0);
	emit sendComm("****************************************\n", 0);
	emit updateWindow();

	clearBuffer();

	// Turn on the Zone One LED
	check_Z1 = true;
	ledOff(8);
	ledOn(zoneOne);

	cout << "before loop" << endl;

	while(exp_running) {
		//scanning();
		if(ard->readCommand()) {

			int compare = ard->lastComm.compare(zoneTwoBroken);
			cout << "compare: " << compare << endl;

			// Rat chooses
			if ((ard->lastComm.compare(zoneOneBroken) == 0) && check_Z1)
			{
				check_Z1 = false;
				check_Z2 = true;

				QString qstr = QString::fromStdString(ard->lastLine);
				emit sendComm(qstr, 1);

				//playSound();
				dispense(zoneOne);

				ledOff(zoneOne);
				ledOn(zoneTwo);
			}
			else if (compare == 0 && check_Z2)
			{
				check_Z1 = true;
				check_Z2 = false;

				QString qstr = QString::fromStdString(ard->lastLine);
				emit sendComm(qstr, 1);

				playSound(500);
				dispense(zoneTwo);

				ledOff(zoneTwo);
				countdown(iti, false);
				ledOn(zoneOne);

				trials--;

				string trialsLeft = "Trials remaining: " + to_string(trials);
			}
		}
		
		if (!trials)
			exp_running = false;
	}

	ledOff(zoneOne);
	ledOff(zoneTwo);
	exp_running = false;
	
	emit naturalEnd();
	emit sendComm("\n****************************************", 0);
	emit sendComm("*************Experiment End*************", 0);
	emit sendComm("****************************************", 0);
	emit updateWindow();
	return;

}


void expCircleThread::startCircle_Tr2(int trials, int zoneOne, int iti, int port, int baud)
{
	ard = new Arduino(port, baud);
	ard->setTimeout_us(500);

	cout << "Zone one: " << zoneOne << endl;

	bool check_Z1 = false;
	bool check_Z2 = false;
	cout << "Starting Trial One" << endl;
	exp_running = true;
	int trialsRun = 0;

	// Set up zone one and zone two with given zone
	int zoneTwo = (zoneOne+4) % 8;

	string zoneOneBroken = "";
	string zoneTwoBroken = "";

	int zoneOneDeg;
	int zoneTwoDeg;

	zoneOneDeg = zoneOne * 45;
	zoneTwoDeg = zoneTwo * 45;

	if (zoneOneDeg < 100 && zoneOneDeg != 0)
		zoneOneBroken = "0" + to_string(zoneOneDeg) + " Broken";
	else if (zoneOneDeg == 0)
		zoneOneBroken = "000 Broken";
	else
		zoneOneBroken = to_string(zoneOneDeg) + " Broken";

	if (zoneTwoDeg < 100 && zoneTwoDeg != 0)
		zoneTwoBroken = "0" + to_string(zoneTwoDeg) + " Broken";
	else if (zoneTwoDeg == 0)
		zoneTwoBroken = "000 Broken";
	else
		zoneTwoBroken = to_string(zoneTwoDeg) + " Broken";

	cout << "Zone One: " << zoneOne << " = " << zoneOneDeg << " degrees" << endl;
	cout << "Zone Two: " << zoneTwo << " = " << zoneTwoDeg << " degrees" << endl;

	emit sendComm("************Begin Training 2************", 0);
	emit sendComm("****************************************\n", 0);
	emit updateWindow();

	clearBuffer();

	// Turn on the Zone One LED
	bool memorize = false; // tracks when rat should memorize pattern/lights should be off but checking
	check_Z1 = true;

	ledOff(8);
	ledOn(zoneOne);

	while(exp_running)
	{
		if(ard->readCommand())
		{

			// Rat chooses
			if ((ard->lastComm.compare(zoneOneBroken) == 0) && check_Z1)
			{
				check_Z1 = false;
				check_Z2 = true;

				QString qstr = QString::fromStdString(ard->lastLine);
				emit sendComm(qstr, 1);

				//playSound();
				dispense(zoneOne);

				ledOff(zoneOne);
				if (!memorize)
					ledOn(zoneTwo);
			}
			else if ((ard->lastComm.compare(zoneTwoBroken) == 0) && check_Z2)
			{
				if (trialsRun%2 == 0) // every other trial don't turn lights on
					memorize = true;
				else if (trialsRun != 2*trials) // if not finished
					memorize = false;

				check_Z1 = true;
				check_Z2 = false;

				QString qstr = QString::fromStdString(ard->lastLine);
				emit sendComm(qstr, 1);

				//playSound();
				dispense(zoneTwo);

				ledOff(zoneTwo);
				
				trialsRun++;
				cout << "trailsRun " << trialsRun << endl;
				if (trialsRun%2 == 0) // every fourth trial, run iti
					countdown(iti, 0);
				if (!memorize)
					ledOn(zoneOne);

				cout << "Remaining trials: " << trials - trialsRun << endl;
			}
		}
		if (trialsRun == 2*trials)
			exp_running = false;
	}

	ledOff(zoneOne);
	ledOff(zoneTwo);
	exp_running = false;
	
	emit naturalEnd();
	emit sendComm("\n****************************************", 0);
	emit sendComm("*************Experiment End*************", 0);
	emit sendComm("****************************************", 0);
	emit updateWindow();
	return;

}


/**
 *-----------------------------------------------------------
 *------------------Functions for Arduino--------------------
 *-----------------------------------------------------------
 *-----------------------------------------------------------
 */

 /**
 *-----------------------------------------------------------
 *------------------Functions for Arduino--------------------
 *-----------------------------------------------------------
 *-----------------------------------------------------------
 */

/*
 *	======== countdown ========
 *
 *  param status 
 *      1st param (int) = time to countdown
 *
 */
 /*
void expCircleThread::countdown(int t)
{
	bool reading = true;
	//bool reading2 = true;
	stringstream sstm;

	string comm_st = "COUNTDOWN ";
	string expected;
	string expectedCancel;

	expected = "Timer Ended";
	expectedCancel = "Timer Canceled";
	sstm << comm_st << t << " " << 0;
	//cout<<sstm.str();

	string new_input = sstm.str();

	ard->sendCommand(new_input);

	while(reading){
		if(ard->readCommand()){
			QString qstr = QString::fromStdString(ard->lastLine);

			cout << ard->lastComm << endl;
			if(ard->lastComm.compare(expected) == 0){
				emit sendComm(qstr, 1);
				reading = false;
			}
		}
	}
}
*/


void expCircleThread::countdown(int t, bool spkr)
{
	bool reading = true;
	stringstream sstm;

	string comm_st = "COUNTDOWN ";
	string expected;
	string expectedCancel;

	if(spkr)
	{
		expected = "Countdown Ended";
		expectedCancel = "Countdown Canceled";
		sstm << comm_st << t << " " << 1;
		//cout<<sstm.str();
	}
	else
	{
		expected = "Timer Ended";
		expectedCancel = "Timer Canceled";
		sstm << comm_st << t << " " << 0;
		//cout<<sstm.str();
	}

	string new_input = sstm.str();

	ard->sendCommand(new_input);

	while(reading){
		if(ard->readCommand()){
			QString qstr = QString::fromStdString(ard->lastLine);

			if(isDis){
				if(ard->lastComm.compare(_disabled1) != 0 && ard->lastComm.compare(_disabled2) != 0){
					emit sendComm(qstr, 1);
				}
			}
			else{
				emit sendComm(qstr, 1);
			}

			cout << ard->lastComm << endl;
			if(ard->lastComm.compare(expected) == 0){
				reading = false;
			}
		}
	}
}











bool expCircleThread::signAndWait(int pos, int state)
{
 	bool triggered = false;

	// convert position to corresponding angle
	int angle = 0;
	for (int i = 0; i < pos; i++)
		angle += 45;

 	stringstream sstrm;
 	string comm1 = "LED ";
 	sstrm << comm1 << state << " " << angle;
 	string out1 = sstrm.str();

 	// load expected results to string
	string stateString = (state == 1? "On" : "Off");
	string small = (angle < 100)? "0" : "";
	string expected = "Turning "+ stateString + " LED " + small + std::to_string(angle) + " degree";

	// Beam Broken
	string startBeamBroken = "";
	if (angle < 100 && angle != 0)
		startBeamBroken = "0" + to_string(angle) + " Broken";
	else if (angle == 0)
		startBeamBroken = "000 Broken";
	else
		startBeamBroken = to_string(angle) + " Broken";

	// send command
	ard->sendCommand(out1);

	// Verify arduino comm
	bool reading = true;
	while(reading) {
		if(ard->readCommand()) {
			if (ard->lastComm.compare(expected) == 0)
				reading = false;
			else
				triggered = false;
			
			if (state == 1) {
				QString qstr = QString::fromStdString(ard->lastLine);
				emit sendComm(qstr, 1);

			}
		}
	}

	bool checkBreakBeam = true;
	while(checkBreakBeam){
		if(ard->readCommand()){
			if(ard->lastComm.compare(startBeamBroken) == 0){
				triggered = true;
				ledOff(pos);
				emit sendComm("Start Zone Broken", 0);
				checkBreakBeam = false;

			}
		}
	}

	return triggered;
}

void expCircleThread::zoneForTrials(int startZone, int endZone)
{

}

bool expCircleThread::scanning()
{
	bool scanned = true;
	stringstream sstrm;
	string comm = "SCAN";
	sstrm << comm;
	string new_input = sstrm.str();

	string expected  = "Broken";


	ard->sendCommand(new_input);

	bool reading = true;
	while(reading) {
		if(ard->readCommand()){
			string ardComm = ard->lastComm;
			size_t pos = ardComm.find("Broken");
			string compStr = ardComm.substr(pos);
			if(compStr.compare(expected) == 0)
				reading = false;

			QString qstr = QString::fromStdString(ard->lastLine);
			emit sendComm(qstr, 1);		
		}
	}
	return scanned;
}


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
	int angle = 0;
	for (int i = 0; i < pos; i++)
		angle += 45;

	// load command to string
	stringstream sstrm;
	string comm = "REWARD ";
	sstrm << comm << angle;
	string new_input = sstrm.str();

	// load expected results to string
	string small = (angle < 100)? "0" : "";
	string zero = (angle == 0)? "0" : "";
	string expected = "Dispensing Reward " + small + zero + std::to_string(angle) + " degree";

	// send command
	ard->sendCommand(new_input);

	// read arduino comm
	bool reading = true;
	while(reading) {
		if(ard->readCommand()){
			cout << ard->lastComm << endl;
			if (ard->lastComm.compare(expected) == 0)
				reading = false;
			
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
bool expCircleThread::playSound(int freq)
{
	bool played = true;

	// load command to string
	stringstream sstrm;
	string comm = "TONE ";
	sstrm << comm << freq;
	string new_input = sstrm.str();

	// load expected results to string
	string expected = "Playing Sound";

	// send command
	ard->sendCommand(new_input);

	// read arduino comm
	bool reading = true;
	while(reading) {
		if(ard->readCommand()){
			cout << ard->lastComm << endl;
			if (ard->lastComm.compare(expected) == 0)
				reading = false;
			
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

	cout << "pos: " << pos << " and state: " << state << endl;

	if (pos < 0 || pos > 8) {
		cout << "led: Invalid position" << endl;
		return false;
	}

	// convert position to corresponding angle
	int angle = 0;
	for (int i = 0; i < pos; i++)
		angle += 45;

	if (pos == 8)
		angle = 1000;

	// load command to string
	stringstream sstrm;
	string comm = "LED ";
	sstrm << comm << state << " " << angle;
	string new_input = sstrm.str();

	// load expected results to string
	string stateString = (state == 1)? "On" : "Off";
	string small = (angle < 100)? "0" : "";
	string zero = (angle == 0)? "0" : "";
	string expected = "Turning "+ stateString + " LED " + small + zero + std::to_string(angle) + " degree";
	if (pos == 8) // turn all on/off
		expected = "Turning "+ stateString + " All LEDs";

	// send command
	ard->sendCommand(new_input);

	// read arduino comm
	bool reading = true;
	while(reading) {
		if(ard->readCommand()) {
			if (ard->lastComm.compare(expected) == 0)
				reading = false;

			if (state == 1) {
				QString qstr = QString::fromStdString(ard->lastLine);
				emit sendComm(qstr, 1);
			}
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
