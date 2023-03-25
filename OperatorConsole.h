#include <string>
#include "Structure.h"

class OperatorConsole {
	private:
		char message;

	public:
		// Constructor
		OperatorConsole();


		// Thread
		void* threadOpConsole(void* arg);

		// Functions to change parameters
		void changeSpeed(int id, double s);
		void changeAltitude(int id, int a);
		void changePosition(int id, float orient);

		// Functions to send the message to the Computer System
		void sendMessage();

		// Function to display the information of the specific aircraft
		void printDetails(int id);
};
