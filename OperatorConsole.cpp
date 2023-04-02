#include "OperatorConsole.h"
#include "Plane.h"
#include "CommSystem.h"
#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <vector>

using namespace std;

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

void OperatorConsole::storeCommands() {
	string filename;
	string command;

	// Vector that will contain the list of commands from the text file
	vector<string> commands_list;

	// Read through the text file and store the commands in a vector
	ifstream inputFile(filename);
	// Verify if the file can be opened
	if(!inputFile)
	{
		cout << "ERROR: The file cannot be opened." << endl;
		return false;
	}

	cout << "The commands are being read..." << endl;
	// Read the lines of the text file till the end of the file
	while(getline(inputFile, command)) {
		// Make sure there is a command and the line is not empty
		if(command.size() > 0) {
			// Add the command to the vector of commands
			commands_list.push_back(command);
		}
	}
	
	inputFile.close();
	return true;
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

	// Verify which command method needs to be run
	if("changeSpeed") {
		changeSpeed();
	}
	else if("changeAltitude") {
		changeAltitude();
	}
	else if("changePosition") {
		changePosition();
	}
	else {
		cout << "This is an invalid command" << endl;
	}
}
