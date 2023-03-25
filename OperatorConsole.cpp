#include "OperatorConsole.h"
#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <vector>

using namespace std;

// will need to include plane class

OperatorConsole::OperatorConsole() {
	//	receivedComm = pthread_create(&thread, &attribute, communicationMain, NULL);
	// cout << "Creating the operator console thread has failed..."
}

void OperatorConsole::changeSpeed(int id, double s) {
	int opConsoleID;
	Msg2CommSys messageCompSys;
}

void OperatorConsole::changeAltitude(int id, int a) {
	int opConsoleID;
	Msg2CommSys messageCompSys;
}

void OperatorConsole::changePosition(int id, float orient) {
	int opConsoleID;
	Msg2CommSys messageCompSys;
}

void OperatorConsole::sendMessage() {
	int opConsoleID;
	Msg2CommSys messageCompSys;


}

void OperatorConsole::printDetails(int id) {
	int opConsoleID;

}

void* threadOpConsole(void* arg) {
	OperatorConsole &operatorConsole = *((OperatorConsole*)arg);

	// write code to read through a text file containing commands

	cout << "The commands are being read..." << endl;

	// Vector that will contain the list of commands from the text file
	vector<string> commands_list;

	// Verify which command needs to be run
	if("changeSpeed") {

	}
	else if("changeAltitude") {

	}
	else if("changePosition") {

	}
	else {
		cout << "This is an invalid command" << endl;
	}
}
