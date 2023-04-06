#include "OperatorConsole.h"
#include "PlaneClass.h"
#include "CommSystem.h"
#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <vector>

using namespace std;

OperatorConsole::OperatorConsole() {
	receivedComm(false);
	compSystem(nullptr);
}

void OperatorConsole::changeSpeed(int id, double s) {
	PlaneClass* plane = null;

	for(int i = 0; i < plane.size(); i++) {
		if(plane[i].getId() == id) {
			plane = &planes[i];
		}
	}
	// Verify if the plane exists in the list of planes
	if(plane = null) {
		cout << "This aircraft is not found" << endl;
	}
	// Change the speed to the new one
	plane -> setSpeed(s);
}

void OperatorConsole::changeAltitude(int id, int a) {
	PlaneClass* plane = null;
	for(int i = 0; i < plane.size(); i++) {
		if(plane[i].getId() == id) {
			plane = &plane[i];
		}
	}
	// Verify if the plane exists in the list of planes
	if(plane == null) {
		cout << "This aircraft is not found" << endl;
	}
	// Change the altitude to the new one
	plane -> setAltitude(a);
}

void OperatorConsole::changePosition(int id, float orient) {
	PlaneClass* plane = null;
	for(int i = 0; i < plane.size(); i++) {
		if(plane[i].getId() == id) {
			plane = &plane[i];
		}
	}
	// Verify if the plane exists in the list of planes
	if(plane == null) {
		cout << "This aircraft is not found" << endl;
	}
	// Change the position/orientation to the new one
	plane -> setOrientation(orient);
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
}

void OperatorConsole::fetchPlaneData(Plane* plane) {
	Plane data;
}

void OperatorConsole::sendMessage(CompSystem* compSystem, int planeID) {
	this->compSystem = compSystem;
}

void OperatorConsole::printDetails(int id) {

	cout << "The aircraft's ID is the following: " << getId() << endl;
	cout << "The aircraft's new flight level: " << getAltitude() << endl;
	cout << "The aircraft's new speed: " << getSpeed() << endl;
	cout << "The aircraft's new position: " << getPosition() << endl;
}

void* OperatorConsole::threadOpConsole(void* arg) {
	// OperatorConsole &operatorConsole = *((OperatorConsole*)arg);

	// Run through the commands for a specific plane ID until it is no longer this ID that sends commands
	Plane planeID;

	while(planeID) {

	}

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
