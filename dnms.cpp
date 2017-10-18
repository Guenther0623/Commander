#include "dnms.h"
//#include "arduino.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

DNMS::DNMS()
{
	//ard = new Arduino();

	rew_amt = -1;

	delay_max = -1;
	delay_min = -1;
	delay_interval = -1;

	loopStarter = "";

	initialized = false;
}

DNMS::DNMS(int port, int baud)
{
	//ard = new Arduino(port, baud);

	rew_amt = -1;

	delay_max = -1;
	delay_min = -1;
	delay_interval = -1;

	loopStarter = "";

	initialized = false;
}

DNMS::DNMS(int port, int baud, int charSize, int parity, int stopBits, int flowCont)
{
	//ard = new Arduino(port, baud, charSize, paroty, stopBits, flowCont);

	rew_amt = -1;

	delay_max = -1;
	delay_min = -1;
	delay_interval = -1;

	loopStarter = "";

	initialized = false;	
}

DNMS::~DNMS()
{

}

void DNMS::setParameters(int rew, int delMx, int delMn, int delR)
{
	rew_amt = rew;
	delay_max = delMx;
	delay_min = delMn;
	delay_interval = delR;

	initialized = true;
}

void DNMS::startExperiment()
{
	if(!initialized)
		cout<<"Please initialize experiment before running"<<endl;
	else{
		//if(ard->ardExists){
			clearBuffer();
		//}
	}
}

void DNMS::endExperiment()
{

}

void DNMS::clearBuffer()
{
	bool clearing = true;

	while(clearing){
		//if(ard->readCommand() == false)
			clearing = false;
	}
}
