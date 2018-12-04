#include "defines.h"

#include <SerialStream.h>
#include <arduino.h>
#include <iostream>
#include <stdio.h>
#include <cstring>

/*#define _PORT_ACM0 0
#define _PORT_ACM1 1
#define _PORT_USB0 10
#define _PORT_USB1 11

#define _BAUD_4800 4800
#define _BAUD_9600 9600
#define _BAUD_14400 14400
*/
using namespace std;
using namespace LibSerial;

Arduino::Arduino(){
	_port = -1;
	_baudRate = -1;
	_charSize = -1;
	_parity = -1;
	_stopBits = -1;
	_flowCont = -1;

	ardExists = false;
}

Arduino::Arduino(int p, int b){
	_port = p;
	_baudRate = b;

	_charSize = 8;
	_parity = 0;
	_stopBits = 1;
	_flowCont = 0;

	if(_port == _PORT_ACM0){
		arduino.Open("/dev/ttyACM0");
	}
	else if(_port == _PORT_ACM1){
		arduino.Open("/dev/ttyACM1");
	}
	else if(_port == _PORT_USB0){
		arduino.Open("/dev/ttyUSB0");
	}
	else if(_port == _PORT_USB1){
		arduino.Open("/dev/ttyUSB1");
	}

	if(arduino.IsOpen()){
		if(_baudRate == _BAUD_9600)
			arduino.SetBaudRate(SerialStreamBuf::BAUD_9600);
		else if(_baudRate == _BAUD_19200)
			arduino.SetBaudRate(SerialStreamBuf::BAUD_19200);
		else if(_baudRate == _BAUD_4800)
			arduino.SetBaudRate(SerialStreamBuf::BAUD_4800);

		arduino.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
		arduino.SetParity(SerialStreamBuf::PARITY_NONE);
		arduino.SetNumOfStopBits(1);
		arduino.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);
		//arduino.SetVTime(10);
		ardExists = true;
		timeout = 10000;
	}
	else{
		cout<<"ERROR: Arduino not found"<<endl;
		ardExists = false;
	}
	arduino>>noskipws;
}

Arduino::Arduino(int p, int b, int c, int par, int s, int f){
	_port = p;
	_baudRate = b;
	_charSize = c;
	_parity = par;
	_stopBits = s;
	_flowCont = f;

	if(_port == _PORT_ACM0){
		arduino.Open("/dev/ttyACM0");
	}
	else if(_port == _PORT_ACM1){
		arduino.Open("/dev/ttyACM1");
	}

	if(arduino.IsOpen()){
		if(_baudRate == _BAUD_9600)
			arduino.SetBaudRate(SerialStreamBuf::BAUD_9600);
		if(_charSize == 8)
			arduino.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
		if(_parity == 0)
			arduino.SetParity(SerialStreamBuf::PARITY_NONE);
		if(_stopBits == 1)
			arduino.SetNumOfStopBits(1);
		if(_flowCont == 0)
			arduino.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);
		//arduino.SetVTime(10);
		ardExists = true;
		timeout = 5000;
	}
	else{
		cout<<"ERROR: Arduino not found";
	}

	arduino>>noskipws;
}

void Arduino::sendCommand(string comm){
	string::iterator it = comm.end();
	if(*it == 10)
		arduino<<comm;
	else{
		comm += '\n';
		arduino<<comm;
	}
}

bool Arduino::readCommand(){
	//arduino.sync_with_stdio(false);
	int t = timeout;
	//10000000 by default

	size_t posStamp = 0;
	string received = "";
	string stamp_string = "";
	int stamp_int;

	while(arduino.rdbuf()->in_avail() == 0 && t > 0)
	{
		t -= 100;
		//cout<<t<<endl;
		usleep(100);
	}

	if(t<= 0){
		return false;
	}

	else{
		bool reading = true;
	
		while(reading){
			char next_char;
			arduino>>next_char;
			if(next_char != 10)
				received += next_char;
		//check if next_char is '/n'
		//set equal to 10 because 10 is ascii for '/n'
			if(next_char == 10){
				//received += '\n';
				reading = false;
			}
		}

		posStamp = (received.find(", "));
		//cout<<posStamp<<endl;
		//cout<<received.length()<<endl;

		if(posStamp < received.length()){
			posStamp += 2;
			stamp_string = received.substr(posStamp);
			lastTStamp_string = stamp_string;
			stamp_int = atoi(stamp_string.c_str());
			lastTStamp_int = stamp_int;
			lastComm = received.substr(0, posStamp-2);
		}

		cout<<"From Read Function: LastComm is "<<lastComm<<endl;

		lastLine = received;

		return true;
	}
}

string Arduino::parseLastCommand(int plc, char delim){
	string input = lastComm;
	string parse = "";
	size_t pos1 = 0, pos2 = 0;

	if(plc == 1){
		pos1 = input.find(delim);
		parse = input.substr(0, pos1);
	}

	else{
		for(int i = 1; i < plc; i++)
			pos1 = input.find(delim, pos1+1);

		pos2 = input.find(delim, pos1+2);
		parse = input.substr(pos1+1, pos2-pos1-1);
	}

	return parse;
}

string Arduino::parseLastCommand(int plc, char delim1, char delim2){
	string input = lastComm;
	string parse = "";
	size_t pos1 = 0, pos2 = 0;

	if(plc == 1){
		pos1 = input.find(delim1);
		parse = input.substr(0, pos1);
	}

	else{
		for(int i = 1; i < plc; i++)
			pos1 = input.find(delim1, pos1+1);

		pos2 = input.find(delim2, pos1+2);
		parse = input.substr(pos1+1, pos2-pos1-1);
	}

	return parse;
}

void Arduino::setTimeout_us(int t)
{
	timeout = t;
}

void Arduino::close(){
	arduino.Close();
}