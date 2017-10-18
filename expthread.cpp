#include <SerialStream.h>
#include <sstream>
#include <unistd.h>

#include "expthread.h"
#include "arduino.h"
#include "defines.h"


/*
 *	======== startDNMS ========
 *
 */
void expThread::startDNMS(int rew, int lps, int ite, int delMode, int port, int baud)
{
	ard = new Arduino(port, baud);

	int trials = lps;
	int curr_loop = 0;
	exp_running = true;

	int forcedRew = 1;

	bool firstLoop = true;
	bool forcedLoop = true;
	bool corrFlag = false;

	bool md = true;
	bool chc = false;
	bool chcR = false;
	bool chcL = false;
	bool fd = false;

	isDis = false;

	int forcedArray[trials];
	int delayArray[trials];

	initArray_DNMSforced(forcedArray, trials);
	initArray_DNMSdelays(delMode, delayArray, trials);

	shuffleArrays_DNMS(forcedArray, delayArray, trials);

	//set_DNMSVals();

	string beam_M = "Middle Beam Broken";
	string beam_CR = "Right Choice Beam Broken";
	string beam_CL = "Left Choice Beam Broken";
	string beam_FR = "Right End Beam Broken";
	string beam_FL = "Left End Beam Broken";

	clearBuffer();

	disableMutes();

	emit sendComm("************Experiment Start************", 0);
	emit sendComm("****************************************\n", 0);

	//door_Middle(_DOOR_OPEN);
	door_ChoiceBoth(_DOOR_CLOSE);
	door_DelayIn(_DOOR_OPEN);
	door_DelayOut(_DOOR_CLOSE);

	//forcedLoop = true;

	while(exp_running){
		if(ard->readCommand()){
			QString qstr = QString::fromStdString(ard->lastLine);

			if(ard->lastComm.compare(_disabled1) == 0 || ard->lastComm.compare(_disabled2) == 0){
				//DNE
			}

			else if(ard->lastComm.compare(beam_M) == 0 && md)
			{
				emit sendComm(qstr, 1);

				if(forcedLoop)
				{
					door_DelayIn(_DOOR_CLOSE);
					
					if(!firstLoop)
						countdown(ite, false);
					
					else
						firstLoop = false;

					emit sendComm("\n****New Trial Beginning****\n-Sample Response Initiated-", 1);
					
					if(forcedArray[curr_loop] == 0){
						emit sendComm("Sample Side Chosen:  LEFT", 1);
						door_ChoiceLeft(_DOOR_OPEN);
						chcL = true;
					}
					
					else if(forcedArray[curr_loop] == 1){
						emit sendComm("Sample Side Chosen:  RIGHT", 1);
						door_ChoiceRight(_DOOR_OPEN);
						chcR = true;
					}

					door_DelayOut(_DOOR_OPEN);
				}
				else if(!forcedLoop)
				{
					door_DelayIn(_DOOR_CLOSE);
					emit sendComm("-Choice Response Initiated-", 1);
					countdown(delayArray[curr_loop], true);
					door_ChoiceBoth(_DOOR_OPEN);			
					door_DelayOut(_DOOR_OPEN);	
					chc = true;
				}

				enableMute_1(beam_M);

				md = false;
				fd = false;
			}

			else if(ard->lastComm.compare(beam_CR) == 0 && (chc || chcR))
			{
				emit sendComm(qstr, 1);

				if(forcedLoop)
				{
					if(forcedArray[curr_loop] == 1){
						door_ChoiceRight(_DOOR_CLOSE);
						door_DelayOut(_DOOR_CLOSE);
					}
					else if(forcedArray[curr_loop] == 0){
						emit sendComm("ERROR: Rat Activated Wrong Side", 1);
					}
				}
				else if(!forcedLoop)
				{
					door_DelayOut(_DOOR_CLOSE);
					door_ChoiceBoth(_DOOR_CLOSE);
					
					if(forcedArray[curr_loop] == 0)
						corrFlag = true;
					else if(forcedArray[curr_loop] == 1)
						corrFlag = false;					
				}

				enableMute_1(beam_CR);
				enableMute_2(beam_CL);
				
				md = false;
				chcR = false;
				chcL = false;
				chc = false;
				fd = true;				
			}

			else if(ard->lastComm.compare(beam_CL) == 0 && (chc || chcL))
			{
				emit sendComm(qstr, 1);

				if(forcedLoop)
				{
					if(forcedArray[curr_loop] == 0){
						door_ChoiceLeft(_DOOR_CLOSE);
						door_DelayOut(_DOOR_CLOSE);
					}
					else if(forcedArray[curr_loop] == 1){
						emit sendComm("ERROR: Rat Activated Wrong Side", 1);
					}						
				}
				else if(!forcedLoop)
				{
					door_DelayOut(_DOOR_CLOSE);					
					door_ChoiceBoth(_DOOR_CLOSE);

					if(forcedArray[curr_loop] == 0)
						corrFlag = false;
					else if(forcedArray[curr_loop] == 1)
						corrFlag = true;					
				}

				enableMute_1(beam_CR);
				enableMute_2(beam_CL);

				md = false;
				chcR = false;
				chcL = false;
				chc = false;
				fd = true;				
			}

			else if(ard->lastComm.compare(beam_FR) == 0 && fd)
			{				
				emit sendComm(qstr, 1);

				if(forcedLoop)
				{
					dispensePell(forcedRew);
					forcedLoop = false;
					emit sendComm("-Sample Response Ended-\n", 1);

					door_DelayIn(_DOOR_OPEN);
				}

				else if(!forcedLoop)
				{
					if(corrFlag){
						emit sendComm("Rat Chose Correctly", 1);
						dispensePell(rew);
					}
					else{
						emit sendComm("Rat Chose Incorrectly", 1);
					}

					emit sendComm("-Choice Response Ended-\n", 1);

					forcedLoop = true;
					door_DelayIn(_DOOR_OPEN);

					curr_loop++;
				}
				enableMute_1(beam_FR);
				enableMute_2(beam_FL);

				md = true;
				chcR = false;
				chcL = false;
				chc = false;
				fd = false;				
			}

			else if(ard->lastComm.compare(beam_FL) == 0 && fd)
			{
				emit sendComm(qstr, 1);

				if(forcedLoop)
				{
					dispensePell(forcedRew);
					forcedLoop = false;
					emit sendComm("-Sample Response Ended-\n", 1);

					door_DelayIn(_DOOR_OPEN);
				}

				else if(!forcedLoop)
				{
					if(corrFlag){
						emit sendComm("Rat Chose Correctly", 1);
						dispensePell(rew);
					}
					else{
						emit sendComm("Rat Chose Incorrectly", 1);
					}

					emit sendComm("-Choice Response Ended-\n", 1);

					forcedLoop = true;
					door_DelayIn(_DOOR_OPEN);

					curr_loop++;
				}
				enableMute_1(beam_FR);
				enableMute_2(beam_FL);	

				md = true;
				chc = false;
				fd = false;							
			}
		}

		if(curr_loop == trials){
			exp_running = false;
			emit sendComm("\n**All Trials Completed**", 1);
		}
	}

	emit sendComm("\n****************************************", 0);
	emit sendComm("*************Experiment End*************", 0);
	emit sendComm("****************************************", 0);
}


/*
 *	======== startRand ========
 *
 */
void expThread::startRand()
{

}


/*
 *	======== startFish ========
 *
 */
void expThread::startFish()
{

}

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
 *      2nd param (bool) = true if sound plays
 * 					     = false if no sound
 */
void expThread::countdown(int t, bool spkr)
{
	bool reading = true;
	//bool reading2 = true;
	stringstream sstm;

	string comm_st = "COUNTDOWN ";
	string expected;

	if(spkr)
	{
		expected = "Countdown Ended";
		sstm << comm_st << t << " " << 1;
		//cout<<sstm.str();
	}
	else
	{
		expected = "Timer Ended";
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
		//emit checkLogStatus();
	}


}

/*
 *	======== door_ChoiceBoth ========
 *  
 *  param status 1-close 0-open
 */
void expThread::door_ChoiceBoth(int state)
{
	bool reading = true;
	stringstream sstm;

	//Create a string with command for arduino
	string comm_st = "CHOICE_BOTH ";

	//Create a string with command expecting back
	string expected;
	if(state == _DOOR_OPEN)
		expected = "Opening Both Choice Doors";
	else if(state == _DOOR_CLOSE)
		expected = "Closing Both Choice Doors";

	//Put command and state in string sstm
	sstm << comm_st << state;
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

			if(ard->lastComm.compare(expected) == 0){
				reading = false;
			}
		}
	}
}


/*
 *	======== door_ChoiceRight ========
 *  
 *  param status 1-close 0-open
 */
void expThread::door_ChoiceRight(int state)
{
	bool reading = true;
	stringstream sstm;

	//Create a string with command for arduino
	string comm_st = "CHOICE_RIGHT ";

	//Create a string with command expecting back
	string expected;
	if(state == _DOOR_OPEN)
		expected = "Opening Right Choice Door";
	else if(state == _DOOR_CLOSE)
		expected = "Closing Right Choice Door";
	
	//Put command and state in string stream
	sstm << comm_st << state;

	string new_input = sstm.str();

	//Send command to arduino
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
		//emit checkLogStatus();
	}
}


/*
 *	======== door_ChoiceLeft ========
 *
 *  param status 1-close 0-open
 */
void expThread::door_ChoiceLeft(int state)
{
	bool reading = true;
	stringstream sstm;

	//Create a string with command for arduino
	string comm_st = "CHOICE_LEFT ";

	//Create a string with command expecting back
	string expected;
	if(state == _DOOR_OPEN)
		expected = "Opening Left Choice Door";
	else if(state == _DOOR_CLOSE)
		expected = "Closing Left Choice Door";
	
	//Put command and state in string stream
	sstm << comm_st << state;

	string new_input = sstm.str();

	//Send command to arduino
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
		//emit checkLogStatus();
	}
}


/*
 *	======== door_DelayIn ========
 *
 *  param status 1-close 0-open
 */
void expThread::door_DelayIn(int state)
{
	bool reading = true;
	stringstream sstm;

	//Create a string with command for arduino
	string comm_st = "DELAY_IN ";

	//Create a string with command expecting back
	string expected;
	if(state == _DOOR_OPEN)
		expected = "Opening Delay Entrance";
	else if(state == _DOOR_CLOSE)
		expected = "Closing Delay Entrance";
	
	//Put command and state in string stream
	sstm << comm_st << state;

	string new_input = sstm.str();

	//Send command to arduino
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
		//emit checkLogStatus();
	}
}


/*
 *	======== door_DelayOut ========
 *
 *  param status 1-close 0-open
 */
void expThread::door_DelayOut(int state)
{
	bool reading = true;
	stringstream sstm;

	//Create a string with command for arduino
	string comm_st = "DELAY_OUT ";

	//Create a string with command expecting back
	string expected;
	if(state == _DOOR_OPEN)
		expected = "Opening Delay Exit";
	else if(state == _DOOR_CLOSE)
		expected = "Closing Delay Exit";
	
	//Put command and state in string stream
	sstm << comm_st << state;

	string new_input = sstm.str();

	//Send command to arduino
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
		//emit checkLogStatus();
	}
}

/*
 *	======== dispensePell ========
 *
 * 	param status - rew = num pellets to dispense
 */
void expThread::dispensePell(int rew)
{
	bool reading = true;
	stringstream sstm;

	string comm_st = "DISP ";
	string expected = "Reward Dispensed";

	sstm << comm_st << rew;

	string input = sstm.str();

	ard->sendCommand(input);

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
		//emit checkLogStatus();
	}
}

/*
 *	======== dispenseRight ========
 *
 * 	param status
 */
void expThread::dispenseRight(int state)
{
	bool reading = true;
	stringstream sstm;

	//Create a string with command for arduino
	string comm_st = "DISP ";

	//Create a string with command expecting back
	string expected = "Dispensing";

	//Put command and state in string stream
	sstm << comm_st << state;

	string new_input = sstm.str();

	//Send command to arduino
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
		//emit checkLogStatus();
	}
}


/*
 *	======== dispenseLeft ========
 *
 *	param status
 */
void expThread::dispenseLeft(int state)
{
	bool reading = true;
	stringstream sstm;

	//Create a string with command for arduino
	string comm_st = "DISP ";

	//Create a string with command expecting back
	string expected = "Dispensing";
	
	//Put command and state in string stream
	sstm << comm_st << state;

	string new_input = sstm.str();

	//Send command to arduino
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
		//emit checkLogStatus();
	}
}


/*
 *	======== led_Left ========
 * 
 *	param status
 */
void expThread::led_Left(int state)
{
	bool reading = true;
	stringstream sstm;

	//Create a string with command for arduino
	string comm_st = "LED_LEFT ";

	//Create a string with command expecting back
	string expected = "Left LED";
	
	//Put command and state in string stream
	sstm << comm_st << state;

	string new_input = sstm.str();

	//Send command to arduino
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
		//emit checkLogStatus();
	}
}


/*
 *	======== led_Right ========
 * 
 *	param status
 */
void expThread::led_Right(int state)
{
	bool reading = true;
	stringstream sstm;

	//Create a string with command for arduino
	string comm_st = "LED_RIGHT ";

	//Create a string with command expecting back
	string expected = "Right LED";
	
	//Put command and state in string stream
	sstm << comm_st << state;

	string new_input = sstm.str();

	//Send command to arduino
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
		//emit checkLogStatus();
	}
}


/*
 *	======== led_Mid ========
 * 
 *	param status
 */
void expThread::led_Mid(int state)
{
	bool reading = true;
	stringstream sstm;

	//Create a string with command for arduino
	string comm_st = "LED_MID ";

	//Create a string with command expecting back
	string expected = "Middle LED";
	
	//Put command and state in string stream
	sstm << comm_st << state;

	string new_input = sstm.str();

	//Send command to arduino
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
		//emit checkLogStatus();
	}
}


/*
 *	======== forceDelay ========
 *
 *	param status
 */
void expThread::forceDelay(int state)
{
	bool reading = true;
	stringstream sstm;

	//Create a string with command for arduino
	string comm_st = "DELAY ";

	//Create a string with command expecting back
	string expected = "Force Delay";
	
	//Put command and state in string stream
	sstm << comm_st << state;

	string new_input = sstm.str();

	//Send command to arduino
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
		//emit checkLogStatus();
	}
}


/*
 *	======== enableMute_1 ========
 */
void expThread::enableMute_1(string dis)
{
	isDis = true;
	_disabled1 = dis;
}

/*
 *	======== enableMute_2 ========
 */
void expThread::enableMute_2(string dis)
{
	isDis = true;
	_disabled2 = dis;
}


/*
 *	======== disableMute ========
 */
void expThread::disableMutes()
{
	isDis = false;
	_disabled1 = "";
	_disabled2 = "";
}

void expThread::clearBuffer()
{
	bool clearing = true;

	while(clearing){
		if(ard->readCommand() == false)
			clearing = false;
	}
}

void expThread::initArray_DNMSforced(int* arr, int vals)
{
	int left = 0;
	int right = 1;
	int i = 0;
	do{
		arr[i] = left;
		i++;
	}while(i < vals/2);

	do{
		arr[i] = right;
		i++;
	}while(i < vals);
}

void expThread::initArray_DNMSdelays(int mode, int* arr, int vals)
{
	//int inc = vals/8;
	if(mode == _0X2X8X32)
	{
		int i = 0;
		do{
			arr[i] = 0;
			arr[i+1] = 2;
			arr[i+2] = 8;
			arr[i+3] = 32;
			i += 4;
		}while(i < vals);
	}
	else if(mode == _0X4X16X32)
	{
		int i = 0;
		do{
			arr[i] = 0;
			arr[i+1] = 4;
			arr[i+2] = 16;
			arr[i+3] = 32;
			i += 4;
		}while(i < vals);
	}
}

void expThread::shuffleArrays_DNMS(int* arr1, int* arr2, int vals)
{
	srand( time(NULL));
	for(int i = 0; i < vals; i++)
	{
		int j = rand() % (i+1);
		swap(&arr1[i], &arr1[j]);
		swap(&arr2[i], &arr2[j]);
	}
}

void expThread::shuffleArray(int* arr, int vals)
{
	srand( time(NULL));
	for(int i = 0; i < vals; i++)
	{
		int j = rand() % (i+1);
		swap(&arr[i], &arr[j]);
	}
}

void expThread::swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}