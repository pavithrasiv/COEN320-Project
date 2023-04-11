#include <string>
#include "Structure.h"
#include "Plane.h"
#include "CompSystem.h"

class OperatorConsole {
	private:
		char message;

	public:
		// Constructor
		OperatorConsole();

		// Thread
		void* threadOpConsole(void* arg);

		// Store the commands in a vector
		void storeCommands();

		// Get the information of the plane that is requesting for commands
		void fetchPlaneData();

		// Function to send messages to the Computer System
		void sendMessage(CompSystem* compSystem);

		// Function to display the information of the specific aircraft
		void printDetails(int id);
};
