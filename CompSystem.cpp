#include "CompSystem.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include "Structure.h"
#include <pthread.h>

//the communication is going to be done in teh same manner as CommSystem
pthread_t createComputerSysThread() {
	
    int receivedComm;
	pthread_t thread;
	pthread_attr_t attr;

	/* Initialize attributes */
	receivedComm = pthread_attr_init(&attr);

	/* Set detach state */
	receivedComm = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* Creating thread with constructed attribute object */
	receivedComm = pthread_create(&thread, &attr, CompSystemMain, NULL);


	return thread;
}

// This is the main body of the CommSys thread
void * CompSystemMain(void *arg) {

    cout << "the thread for the computer system is started..." << endl;


	string nameChannel= "ComputerChannel";

    name_attach_t *pathChannel;
	Msg2ComputerSys message;
	int receivedID;

	pathChannel = name_attach(NULL, nameChannel.c_str(), 0);

    //if there is no valid path name, there is going to be an error 
	if(pathChannel == NULL) {
		cout << "Error in the channel name" << endl;
		return NULL;
	}

	while (true) {

        //the received message is going to be stored according to its receivedID, which is going to be used as an identifier
		receivedID = MsgReceive(pathChannel->chid, &message, sizeof(message), NULL);

        cout << "Message received!" << endl;


        //create a method in which we are going to be checking if the formatting of the message is properly done 
		bool messageFormat = properMsgFormat(receivedID, message);

        //if the message is not properly formatted, you are going to be continuing into the loop until you get the right message format 
        if (messageFormat != true){
            continue;
        }

        //check what type of message that is received 

        switch(message.type){
            case OperatorSetCalculationInterval:
                cout << "message from operator is received" << endl;

                //call method that is goinng to perform the calculation of the interval 
                //call method that is going to be replying to the message received

                break;

            case AirplaneRadarUpdate:
                cout << "message from airplane received" << endl;

                //method that is going to be handling the update of the airplane 
                //call method that is going to be replying to the message received

                break;

            case ClockTimerUpdate:
                //method to update the clock timer 
                //call method that is going to be replying to the message received
                //check for collision every 30s 
                collisionCheck();
                break;

            default:
                cout << "An unknown message has been received..." << endl;
                break;
        }


	name_detach(pathChannel, 0);

	return NULL;
}

void collisionCheck(){

    cout << "There is a request to update the clock timer.." << endl;

    //call a variable of clock timer and check every x seconds if there is collision; timer %30

	double collisionTime;

	for (size_t i=0; ; i++) {
		for ( size_t y=i+1; ; y++) {
			//test the matrix in which we are going to be checking for plane collision 
			// if there is a collision, we want to display a warning message
			if (collision == true ) {
                cout << "there is a collision..." << endl; 
                cout << "We are going to be displaying th plane ID1 and ID2, time of collision, position in the 3d matrix, and collision time" << endl;
			}
		}
	}
	//this is where we are going to be repluing to the message 
}