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

// Operator Console constructor definition
OperatorConsole::OperatorConsole() {
	// initialize to not receiving anything from the Communication system
	receivedComm(false);
	// initialize a pointer to the Computer System to send it data
	compSystem(nullptr);
}

// store the commands sent to the Operator console into a log file
void OperatorConsole::storeCommands() {
	// create a log file where the commands will be stored
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
}

// fetch each plane's information
void OperatorConsole::fetchPlaneData(PlaneClass* plane) {
	PlaneClass data;
}

// send all the command details to the Computer System
void OperatorConsole::sendMessage(CompSystem* compSystem) {
	this->compSystem = compSystem;
}

// function definition to allow the display of the aircraft details onto the Display Data class from the Computer System
void OperatorConsole::printDetails(int id) {
	this->compSystem
}

// this is the main function of the Operator Console where the commands will be verified to make sure they are valid
void* OperatorConsole::operatorMain(void* arg) {
	// OperatorConsole &operatorConsole = *((OperatorConsole*)arg);

	// get the plane ID information from the PlaneClass to iterate through each of them for the command executions
	PlaneClass planeID;
	// get the command definitions from the CompSystem class
	CompSystem command;

	// Run through the commands for a specific plane ID until it is no longer this ID that sends commands
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
		// once commands are verified, they can be sent to the Computer System for execution
		sendMessage();
	}
}
