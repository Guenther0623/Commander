#include "expFishThread.h"

/*
 *	======== startFish Experiment========
 *	Contains all experiment logic for fishing hole!!!
 *	Only exits function upon user request
 *	Has three modes:
 *		(1) pMode == _NP_ALL
 *			Non predictive, uses all probabilities 1-100 in steps of 10
 *			Runs # of trials before switching
 *
 *		(2) pMode == _PREDICTIVE
 *			Predictive! Uses all probabilities
 *			Switches when last _TRACKING_HISTORY results equal right % of choices
 *
 *		(3) pMode == _FIFTYFIFTY
 *			Non predictive, only uses probability 50%
 */
void expFishThread::startFish_Exp(int trials, int iti, int pMode, int port, int baud)
{
	/***********************
	** Setup Declarations **
	***********************/

	bool fiftyFifty = ((pMode == _FIFTYFIFTY)? true : false);
	bool predictive = ((pMode == _PREDICTIVE)? true : false);

	// set up arduino
	int spkrFreq = 700;
	int soundDuration = 200;
	ard = new Arduino(port, baud);
	ard->setTimeout_us(500);	
	set_Frequency(spkrFreq);

	// set up random number generator
	std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(1, 10); // define the range

    // set up experiment variables
	exp_running = true;
	bool check_M = false;
	bool check_L = false;
	bool check_R = false;
	string midBroken = "Mid Beam Broken: Begin";
	string rightBroken = "Right Beam Broken";
	string leftBroken = "Left Beam Broken";
	int switchCount = -1; // follows # of switches, this%size = current probability array index
	bool newSwitch = true;
	string hist = "";
	const int TRACKING_HISTORY = 20;

	// set up probability array
	const int size = 9;
	double probabilityL[size];
	double allPossVals[size-1] = {90.0, 80.0, 70.0, 60.0, 40.0, 30.0, 20.0, 10.0};

	double possLVals_h[(int)(size/2)];
	double possLVals_l[(int)(size/2)];
	
	if (fiftyFifty) {
		predictive = false;
		for(int i = 0; i < size; i++)
			probabilityL[i] = 50.0;
	}

	else {
		for (int i = 0; i < size-1; i++)
			if(i < size/2)
				possLVals_h[i] = allPossVals[i];
			else
				possLVals_l[i-(size/2)] = allPossVals[i];

		createProbsArray(probabilityL, possLVals_h, possLVals_l, size);
	}

	emit sendComm("************Experiment Start************", 0);
	emit sendComm("****************************************\n", 0);
	emit updateWindow();

	clearBuffer();

	led_Mid(1);
	led_LeftRight(0);

	check_M = true;

	while (exp_running)
	{
		// Probability switch
		if (newSwitch)
		{
			newSwitch = false;
			switchCount++;
			hist = "";  // reset history

			if (switchCount > 0) {  // aka not the first run
				ostringstream printSwitch;
				printSwitch << switchCount;
				std::string stringSwitch = "\nProbability switch detected, moving to phase #" + printSwitch.str() + "\n";
				QString qstr5 = QString::fromStdString(stringSwitch);
				emit sendComm(qstr5, 1);
			}

			if (switchCount%size >= size) {  // if whole array used, shuffle
				shuffleArrayD(probabilityL, size);
			}

			ostringstream printRatioProbL, printRatioProbR;
			printRatioProbL << probabilityL[switchCount%size];
			printRatioProbR << 100.0 - probabilityL[switchCount%size];
			std::string stringProbL = "Probability for reward on left poke = " + printRatioProbL.str() + "%";
			std::string stringProbR = "Probability for reward on right poke = " + printRatioProbR.str() + "%";
			QString qstr1 = QString::fromStdString(stringProbL);
			QString qstr2 = QString::fromStdString(stringProbR);
			
			emit sendComm(qstr1, 1);
			emit sendComm(qstr2, 1);
			emit sendComm("", 0);
			emit updateWindow();
		}

		if (ard->readCommand())
		{
			// Rat activates start of experiment
			if ((ard->lastComm.compare(midBroken) == 0) && (check_M))
			{
				check_L = true;
				check_R = true;
				check_M = false;

				QString qstr = QString::fromStdString(ard->lastLine);

				emit sendComm("", 0);
				emit sendComm("New Run Started", 1);
				emit sendComm(qstr, 1);
				emit updateWindow();

				led_Mid(0);
				countdown(2, false);
				led_LeftRight(1);
			}

			// Rat activates right poke
			else if ((ard->lastComm.compare(rightBroken) == 0) && (check_R))
			{
				check_L = false;
				check_R = false;
				check_M = true;

				QString qstr = QString::fromStdString(ard->lastLine);
				emit sendComm(qstr, 1);

				// Random match success
				int merand = distr(eng);
				// cout << merand << " ? ";
				// cout << 10-(probabilityL[switchCount%size] / 10.0) << endl;
				if (merand <= 10-(probabilityL[switchCount%size] / 10.0))
				{
					// cout << "award!" << endl;
					play_Sound(soundDuration);
					dispenseRight();
					hist.append("2");
				}

				// Random match fail
				else
				{
					hist.append("0");	
				}

				if (predictive) {
					if (hist.length() >= TRACKING_HISTORY) {
						// Check for switch
						string history = hist.substr(hist.length() - TRACKING_HISTORY, TRACKING_HISTORY); // last 20 experiments
						// cout << "history: " << history << endl;
                    	if ((100 - probabilityL[switchCount%size]) > 50) {
							if ((countChars(history, '2') + countChars(history, '0')) >= 1-(probabilityL[switchCount%size] / 10.0)) { // success count >= success probability
								newSwitch = true;
							}
                    	}
                    	else {
							if ((countChars(history, '2') + countChars(history, '0')) <= 1-(probabilityL[switchCount%size] / 10.0)) { // success count <= success probability
								newSwitch = true;
							}
                        }
					}
				}
				else if (pMode == _NP_ALL) {
                    if (hist.length() == trials)
						newSwitch = true;
				}

				led_LeftRight(0);
				countdown(iti, false);
				led_Mid(1);

			}

			// Rat activates left poke
			else if ((ard->lastComm.compare(leftBroken) == 0) && (check_L))
			{
				check_L = false;
				check_R = false;
				check_M = true;

				QString qstr = QString::fromStdString(ard->lastLine);
				emit sendComm(qstr, 1);

				// Random match success
				int merand = distr(eng);
				// cout << merand << " ? ";
				// cout << probabilityL[switchCount%size] / 10.0 << endl;
				if (merand <= probabilityL[switchCount%size] / 10.0)
				{
					// cout << "award!" << endl;
					play_Sound(soundDuration);
					dispenseLeft();
					hist.append("3");
				}

				// Random match fail
				else
				{
					hist.append("1");	

				}

				if (predictive) {
					if (hist.length() >= TRACKING_HISTORY) {
						// Check for switch
						string history = hist.substr(hist.length() - TRACKING_HISTORY, TRACKING_HISTORY); // last # experiments
						// cout << "history: " << history << endl;
                        if (probabilityL[switchCount%size] > 50) {
							if ((countChars(history, '3') + countChars(history, '1')) >= probabilityL[switchCount%size] / 10.0) { // success count >= success probability
								newSwitch = true;
							}
                        }
                        else {
							if ((countChars(history, '3') + countChars(history, '1')) <= probabilityL[switchCount%size] / 10.0) { // success count <= success probability
								newSwitch = true;
							}
                        }
					}
				}
				else {
                    if ((int)hist.length() == trials)
						newSwitch = true;
				}

				led_LeftRight(0);
				countdown(iti, false);
				led_Mid(1);
			}
		}
	}
	emit sendComm("Shutting down experiment", 0);
	led_Mid(0);
	led_LeftRight(0);
	exp_running = false;
	
	emit naturalEnd();
	emit sendComm("\n****************************************", 0);
	emit sendComm("*************Experiment End*************", 0);
	emit sendComm("****************************************", 0);
	emit updateWindow();
	return;
}


void expFishThread::startFish_Tr1(int trials, int iti, int port, int baud)
{
	int spkrFreq = 700;
	int soundDuration = 200;

	ard = new Arduino(port, baud);
	ard->setTimeout_us(500);
	set_Frequency(spkrFreq);

	exp_running = true;

	std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(1, 10); // define the range

    int training_trials = 0;

	emit sendComm("************Begin Training 1************", 0);
	emit sendComm("****************************************", 0);
	while(training_trials < trials && exp_running) {
		play_Sound(soundDuration);
		if (rand()%2)
			dispenseRight();
		else
			dispenseLeft();
		training_trials++;
		std::cout << "iti: " << iti << std::endl;
		countdown(iti, false);
	}
	exp_running = false;

	emit naturalEnd();
	emit sendComm("\n****************************************", 0);
	emit sendComm("**************Training End**************", 0);
	emit sendComm("****************************************", 0);
	emit updateWindow();
	return;
}

void expFishThread::startFish_Tr2(int trials, int iti, int port, int baud)
{
	int spkrFreq = 700;
	int soundDuration = 200;

	ard = new Arduino(port, baud);
	ard->setTimeout_us(500);
	set_Frequency(spkrFreq);

	exp_running = true;

	int training_trials = 0;

	string rightBroken = "Right Beam Broken";
	string leftBroken = "Left Beam Broken";
	string lastComm = "";

	emit sendComm("************Begin Training 2************", 0);
	emit sendComm("****************************************", 0);
	led_LeftRight(1);

	while (training_trials < trials && exp_running) {
		if (ard->readCommand()) {
			lastComm = ard->lastComm;
			if (lastComm.compare(rightBroken) == 0 || lastComm.compare(leftBroken) == 0) {
				training_trials++;
				QString qstr = QString::fromStdString(ard->lastLine);
				emit sendComm(qstr, 1);

				// reward
				play_Sound(soundDuration);
				if (lastComm.compare(rightBroken) == 0)
					dispenseRight();
				else
					dispenseLeft();

				// prepare next trial
				led_LeftRight(0);
				emit updateWindow();
				countdown(iti, false);
				led_LeftRight(1);
			}
		}
	}
	led_LeftRight(0);
	led_Mid(0);
	exp_running = false;

	emit naturalEnd();
	emit sendComm("\n****************************************", 0);
	emit sendComm("**************Training End**************", 0);
	emit sendComm("****************************************", 0);
	emit updateWindow();
	return;
}

void expFishThread::startFish_Tr3(int trials, int iti, int port, int baud)
{
	int spkrFreq = 700;
	int soundDuration = 200;

	ard = new Arduino(port, baud);
	ard->setTimeout_us(500);
	set_Frequency(spkrFreq);

	exp_running = true;
	bool check_middle = true;

	int training_trials = 0;

	string midBroken = "Mid Beam Broken: Begin";
	string rightBroken = "Right Beam Broken";
	string leftBroken = "Left Beam Broken";
	string lastComm = "";

	emit sendComm("************Begin Training 3************", 0);
	emit sendComm("****************************************", 0);
	led_Mid(1);
	while (training_trials < trials && exp_running) {
		if (trials == 0) // edge case
			break;
		if (ard->readCommand()) {
			lastComm = ard->lastComm;
			if (check_middle && lastComm.compare(midBroken) == 0) {
				check_middle = false;
				QString qstr = QString::fromStdString(ard->lastLine);
				emit sendComm(qstr, 1);

				led_Mid(0);
				led_LeftRight(1);
			}
			else  if (!check_middle && (lastComm.compare(rightBroken) == 0 || ard->lastComm.compare(leftBroken) == 0)) {
				training_trials++;
				check_middle = true;
				QString qstr = QString::fromStdString(ard->lastLine);
				emit sendComm(qstr, 1);

				//reward
				play_Sound(soundDuration);
				if (lastComm.compare(rightBroken) == 0)
					dispenseRight();
				else
					dispenseLeft();

				//setup next trial
				led_LeftRight(0);
				countdown(iti, false);
				led_Mid(1);
			}
		}
	}
	led_Mid(0);
	led_LeftRight(0);
	exp_running = false;

	emit naturalEnd();
	emit sendComm("\n****************************************", 0);
	emit sendComm("**************Training End**************", 0);
	emit sendComm("****************************************", 0);
	emit updateWindow();
	return;
}

void expFishThread::createProbsArray(double* arrTot, double* arrH, double* arrL, int size)
{

	shuffleArrayD(arrL, (int)(size/2));
	shuffleArrayD(arrH, (int)(size/2));

	//alternates starting with high or low values
	int randNumber = (rand() % 2); //the 2 gives values 0 and 1 -> make h or l first value rando
	int j = 0;
	for(int i = 0; i <= 3; i++)
	{
		if (randNumber == 1)
		{
			arrTot[j] = arrH[i];
			j++;
			arrTot[j] = arrL[i];
			j++;
		}
		else
		{
			arrTot[j] = arrL[i];
			j++;
			arrTot[j] = arrH[i];
			j++;
		}
	}

	//this selection randomly puts 50 somewhere
	int ranNumber = (rand( ) % 9);	//random selection of value 0 to 8 to randomly select position

	double temp = 0;
	double hold = arrTot[ranNumber]; //hold rando spot in array value


	for (int i = (ranNumber + 1); i < size; i++)
	{
		temp = arrTot[i];
		arrTot[i] = hold;
		hold = temp;
	}

	arrTot[ranNumber] = 50; //assigns ranNum as 50 post all # in array shifting

	cout << "Probability array is: ";
	for(int i = 0; i < size; i++)
	{
		cout << (int)arrTot[i] << ", ";
	}
}


void expFishThread::shuffleArray(int* arr, int size)
{
	srand( time(NULL) );
	for(int i = 0; i < size; i++)
	{
		int j = rand() % (i+1);
		swap(&arr[i], &arr[j]);
	}
}


void expFishThread::shuffleArrayD(double* arr, int size)
{
	srand( time(NULL) );
	for(int i = 0; i < size; i++)
	{
		int j = rand() % (i+1);
		swapD(&arr[i], &arr[j]);
	}
}


void expFishThread::swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}


void expFishThread::swapD(double* a, double* b)
{
	double temp = *a;
	*a = *b;
	*b = temp;
}


int expFishThread::countChars(string str, char c)
{
	int count = 0;
        for (uint i = 0; i < str.length(); i++)
		if (str.at(i) == c)
			count++;
	return count;
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
void expFishThread::countdown(int t, bool spkr)
{
	bool reading = true;
	//bool reading2 = true;
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



/*
 *	======== set_Frequency ========
 *
 *  param status
 */
void expFishThread::set_Frequency(int inFreq)
{
	bool reading=true;
	stringstream sstm;
	int spkr_freq = inFreq;
	string comm_st = "set_freq ";
	string expected = "Setting frequency";
	sstm<<comm_st<<spkr_freq;

	string new_input = sstm.str();

	ard->sendCommand(new_input);

	while(reading){
		if(ard->readCommand()){
			QString qstr = QString::fromStdString(ard->lastLine);

			if(isDis){
				if(ard->lastComm.compare(_disabled1) != 0){
					//emit sendComm(qstr, 1);
				}
			}
			else{
				//emit sendComm(qstr, 1);
			}

			cout << ard->lastComm << endl;
			if(ard->lastComm.compare(expected) == 0){
				reading = false;
			}
		}
		
	}
}

/*
 *	======== play_Sound ========
 *
 *  param status 
 */
void expFishThread::play_Sound(int duration)
{
	bool reading = true;
	stringstream sstm;
	int soundDur = duration;
	string comm_st = "play_sound ";
	string expected = "Playing sound";
	sstm<<comm_st<<soundDur;

	string new_input = sstm.str();

	ard->sendCommand(new_input);


	while(reading){
		if(ard->readCommand()){
			QString qstr = QString::fromStdString(ard->lastLine);

			if(isDis){
				if(ard->lastComm.compare(_disabled1) != 0){
					//emit sendComm(qstr, 1);
				}
			}
			else{
				//emit sendComm(qstr, 1);
			}

			cout << ard->lastComm << endl;
			if(ard->lastComm.compare(expected) == 0){
				reading = false;
			}
		}
	}
}

/*
 *	======== dispenseRight ========
 *
 * 	param status
 */
void expFishThread::dispenseRight()
{
	bool reading = true;
	stringstream sstm;

	//Create a string with command for arduino
	string comm_st = "dispr";

	//Create a string with command expecting back
	string expected = "Dispensing right";

	//Put command and state in string stream


	//Send command to arduino
	ard->sendCommand(comm_st);

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
void expFishThread::dispenseLeft()
{
	bool reading = true;
	stringstream sstm;

	//Create a string with command for arduino
	string comm_st = "displ";

	//Create a string with command expecting back
	string expected = "Dispensing left";
	
	//Put command and state in string stream

	//Send command to arduino
	ard->sendCommand(comm_st);

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
 *	======== led_LeftRight ========
 * 
 *	param status
 */
void expFishThread::led_LeftRight(int state){
	bool reading = true;
	string input, expected;
	stringstream sstm;

	if (state == 1){
		input ="LR_LED 1";
		expected = "Left and right LEDs ON";
	}
	else if (state == 0){
		input = "LR_LED 0";
		expected = "Left and right LEDs OFF";
	}

	ard->sendCommand(input);

	while(reading){
		if(ard->readCommand()){
			QString qstr = QString::fromStdString(ard->lastLine);

			if(isDis){
				if(ard->lastComm.compare(_disabled1) == 0){
					//emit sendComm(qstr, 1);
				}
			}
			else{
				//emit sendComm(qstr, 1);
			}
			cout << ard->lastComm << endl;
			if(ard->lastComm.compare(expected) == 0){
				reading = false;
			}
		}
	}
}


/*
 *	======== led_Left ========
 * 
 *	param status
 */
void expFishThread::led_Left(int state)
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
 *	======== led_Mid ========
 * 
 *	param status
 */
void expFishThread::led_Mid(int state)
{
	bool reading = true;
	stringstream sstm;

	//Create a string with command for arduino
	string comm_st = "MID_LED ";


	//Create a string with command expecting back
	string expected;

	if (state == 1){
		expected = "Middle LED ON";
	}
	else if (state == 0){
		expected = "Middle LED OFF";
	}	
	//Put command and state in string stream
	sstm << comm_st << state;

	string new_input = sstm.str();

	//Send command to arduino
	ard->sendCommand(new_input);

	while(reading){
		if(ard->readCommand()){
			QString qstr = QString::fromStdString(ard->lastLine);

			if(isDis){
				if(ard->lastComm.compare(_disabled1) != 0){
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

/*
 *	======== led_Right ========
 * 
 *	param status
 */
void expFishThread::led_Right(int state)
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
				if(ard->lastComm.compare(_disabled1) != 0){
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


/*
 *	======== enableMute_1 ========
 */
void expFishThread::enableMute_1(string dis)
{
	isDis = true;
	_disabled1 = dis;
}

/*
 *	======== enableMute_2 ========
 */
void expFishThread::enableMute_2(string dis)
{
	isDis = true;
	_disabled2 = dis;
}


/*
 *	======== disableMute ========
 */
void expFishThread::disableMutes()
{
	isDis = false;
	_disabled1 = "";
	_disabled2 = "";
}

void expFishThread::clearBuffer()
{
	bool clearing = true;

	while(clearing){
		if(ard->readCommand() == false)
			clearing = false;
	}
}


/*
 *	======== scanning ========
 *  Polling for Left and Right Break Beams (Rando)
 */
int expFishThread::scanning(){
	bool reading = true;
	string input, expectedR, expectedL;
	input = "scan";

	int choice = -1;

	expectedR="Right Beam Broken";
	expectedL="Left Beam Broken";

	ard->sendCommand(input);

	while(reading){
		if(ard->readCommand()){
			QString qstr = QString::fromStdString(ard->lastComm);

			if(isDis){
				if(ard->lastComm.compare(_disabled1) != 0){
					emit sendComm(qstr, 1);
				}
			}
			else{
				emit sendComm(qstr, 1);
			}

			if(ard->lastComm.compare(expectedR) == 0){
				choice = 0;
				reading = false;
			}
			else if(ard->lastComm.compare(expectedL) == 0){
				choice = 1;
				reading = false;
			}
		}
	}

	return choice;
}


/*
 *	======== identify ========
 *  Initialization of Rando and Fishing
 */
int expFishThread::identify(){
	bool reading = true;
	string input = "Initialize";
	string expectedRando = "RandoMaze_Initiation";
	string expectedFishing = "FishingHole_Initialization";

	int mazeNumber = -1;

	ard->sendCommand(input);

	while (reading){
		if (ard->readCommand()){
			QString qstr = QString::fromStdString(ard->lastComm);

			if (isDis){
				if (ard->lastComm.compare(_disabled1) != 0){
					emit sendComm(qstr, 1);
				}
			}
			else{
				emit sendComm(qstr, 1);
			}

			if (ard->lastComm.compare(expectedRando) == 0){
				mazeNumber = 1;
				reading = false;
			}
			else if (ard->lastComm.compare(expectedFishing) == 0){
				mazeNumber = 0;
				reading = false;
			}
		}
	}
	return mazeNumber;
}

/*
 * ========== End Functions for Arduino  =====================
 */