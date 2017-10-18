#ifndef ARDUINO_H
#define ARDUINO_H

#include <SerialStream.h>
#include <SerialStreamBuf.h>
#include <SerialPort.h>
#include <string>

using namespace std;

class Arduino
{
private:
	LibSerial::SerialStream arduino;
	int _port;
	int _baudRate;
	int _charSize;
	int _parity;
	int _stopBits;
	int _flowCont;
	int timeout;

public:
	Arduino();
	Arduino(int, int);
	Arduino(int, int, int, int, int, int);

	void sendCommand(string);
	bool readCommand(void);
	string parseLastCommand(int, char);
	string parseLastCommand(int, char, char);
	void setTimeout_us(int);
	void close();
	
	bool ardExists;
	int lastTStamp_int;
	string lastTStamp_string;
	string lastComm, lastLine;
};

#endif