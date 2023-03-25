#include "OperatorConsole.h"
#include <iostream>
#include <string>
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

	messageCompSys.message;
}

void OperatorConsole::printDetails(int id) {
	int opConsoleID;

}
