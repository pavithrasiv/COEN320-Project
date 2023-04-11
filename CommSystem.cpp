#include "CommSystem.h"
#include "PlaneClass.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <pthread.h>

using namespace std;

typedef struct _pulse msg_header_t;

void * communicationMain() {
	cout << "The communication has begun..." << endl;

	string nameChannel = "CommunicationChannel";

	cout << "Creating messaging system with" << nameChannel << endl;

	// To register a name in the pathname space and create a channel
	name_attach_t *pathChannel;

	PlaneClass message();

	int receivedID;

	pathChannel = name_attach(NULL, nameChannel.c_str(), 0);

	if(pathChannel == NULL) {
		cout << "Error in the channel name" << endl;
		return NULL;
	}

	while(true) {
		// Use predefined function MsgReceive()
		receivedID = MsgReceive(pathChannel->chid, &message, message.size(), NULL);
		cout << "Message received!" << endl;
//		include the forwarding message by operator to airplane
	}
	return NULL;
}



void disconnectFromChannel(int coid) {
    int status = ConnectDetach(coid);
    if (status == -1) {
        // Handle error
        cout<< "Error detaching connection" << endl;
    }
}


pthread_t createCommunication() {
	int receivedComm;
	pthread_t thread;
	pthread_attr_t attribute;

	receivedComm = pthread_attr_init(&attribute);

	receivedComm = pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_JOINABLE);


	return thread;

}

void sendMessage(int chid, const void *msg, int size) {

	//plane, sending id and client id 

    int receivedID;
    struct _msg_info info;

    // Send message using MsgSend
    int status = MsgSend(chid, msg, size, NULL, 0);

    // Check for errors
    if (status == -1) {
        std::cout << "Error sending message" << endl;
        return;
    }

    // Wait for reply
    receivedID = MsgReceive(chid, NULL, 0, &info);

    // Check for errors
    if (receivedID == -1) {
        std::cout << "Error receiving reply" << endl;
        return;
    }
}
