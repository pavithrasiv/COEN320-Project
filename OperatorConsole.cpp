#include "OperatorConsole.h"
#include "PlaneClass.h"
#include "CommSystem.h"
#include "CompSystem.h"
#include <iostream>
#include "fstream"
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <vector>

using namespace std;

OperatorConsole::OperatorConsole() {
	receivedComm(false);
	compSystem(nullptr);
}

void OperatorConsole::storeCommands() {
	string filename = "log.txt";
	string command;

	// Vector that will contain the list of commands sent to the operator console
	// The commands logged in the log file will be seen by the computer system and enable the controller to pursue with the commands
	vector<string> commands_list;

	//create the log file
	ofstream inputFile(filename);
	//write the commands into the log file
	inputFile << commands_list ; 
	//close the log file once everything is written
	inputFile.close();

// 	// Read through the text file and store the commands in a vector
// 	ifstream inputFile(filename);
// 	// Verify if the file can be opened
// 	if(!inputFile)
// 	{
// 		cout << "ERROR: The file cannot be opened." << endl;
// 	}

// 	cout << "The commands are being read..." << endl;
// 	// Read the lines of the text file till the end of the file
// 	while(getline(inputFile, command)) {
// 		// Make sure there is a command and the line is not empty
// 		if(command.size() > 0) {
// 			// Add the command to the vector of commands
// 			commands_list.push_back(command);
// 		}
// 	}
	
// 	inputFile.close();
// }

void OperatorConsole::fetchPlaneData(Plane* plane) {
	Plane data;
}

void OperatorConsole::sendMessage(CompSystem* compSystem, int planeID) {
	this->compSystem = compSystem;
}

void OperatorConsole::printDetails(int id) {
	this->compSystem
}

void* OperatorConsole::threadOpConsole(void* arg) {
	// OperatorConsole &operatorConsole = *((OperatorConsole*)arg);

	// Run through the commands for a specific plane ID until it is no longer this ID that sends commands
	PlaneClass planeID;
	CompSystem command;

	while(planeID) {
		// Verify which command method needs to be run and make sure it is a possible input
		if("changeSpeed") {
			command.changeSpeed();
		}
		else if("changeAltitude") {
			command.changeAltitude();
		}
		else if("changePosition") {
			command.changePosition();
		}
		else {
			cout << "This is an invalid command" << endl;
		}
	}
}
