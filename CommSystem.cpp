#include "CommSystem.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include "Structure.h"
#include <pthread.h>

using namespace std;

void sendMessageToAirplane(const char* planeChannel, Msg2Airplane message){
	const int serverConnectionID = name_open(airplaneChannel, 0);
	/*
	 * We want to close the connection after it is being used, which is going to be 
	 *freeing up some space after the message is sent
	*/
    if (serverConnectionID == -1) {
        cout << "Communication system error: " << planeChannel <<  " channel was not created\n" << endl;
        return false;
    }

	cout << "Communication system is sending a message: " << message.type << "\n" << "to plane: " << message.id << "on channel: " << planeChannel << endl;
    
	//int MsgSend(int coid, const void *msg_ptr, int msg_size, void *reply_ptr, int reply_size);
	const int sendResult = MsgSend(serverConnectionID, &message, sizeof(message), nullptr, 0);

    if (sendResult == -1) {
		cout << "Communication system error: " << planeChannel << " channel message was not sent /n" << endl;
        return false;
    }

	name_close(serverConnectionID);

    return true;
}



void * communicationMain() {
	cout << "The communication has begun..." << endl;

	string nameChannel = "CommunicationChannel";

	cout << "Creating messaging system with" << nameChannel << endl;

	// To register a name in the pathname space and create a channel
	name_attach_t *pathChannel;

	Msg2CommSys message;

	int receivedID;

	pathChannel = name_attach(NULL, nameChannel.c_str(), 0);

	if(pathChannel == NULL) {
		cout << "Error in the channel name" << endl;
		return NULL;
	}

	while(true) {
		// Use predefined function MsgReceive()
		receivedID = MsgReceive(pathChannel->chid, &message, sizeof(message), NULL);
		cout << "Message received!" << endl;
//		include the forwarding message by operator to airplane
	}
	return NULL;
}

pthread_t createCommunicationThread() {
	int receivedComm;
	pthread_t thread;
	pthread_attr_t attribute;

	receivedComm = pthread_attr_init(&attribute);

	receivedComm = pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_JOINABLE);

//	receivedComm = pthread_create(&thread, &attribute, communicationMain, NULL);
//
//	if (receivedComm != 0) {
//	        cerr << "Failed to create thread" << endl;
//	        exit(EXIT_FAILURE);
//	    }
	return thread;

}
