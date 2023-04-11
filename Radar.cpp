#include "PlaneClass.h"


//Thread that receives message (server) needs a channel, and thread that sends message
//Must create a connection that attaches to the channel
#define ATTACH_POINT "my_channel"

//Message header is at least a pulse
//This identifies the message that is being sent and received
//Unique ID for it basically
typedef struct _pulse msg_header_t;

//Format of the data
typedef struct _my_data {
	msg_header_t hdr;
	double data;
} my_data_t;

//Format of plane
typedef struct _plane_data {
	msg_header_t hdr;
	int id;
	int velocity[3];
	int position[3];
    bool outsideAirspace;
    bool ArrivedYet;
} plane_data_t;


void* radarserverthread(void*) {
	//Name of attach value for the channel and connection
	name_attach_t *name_attach_t;

	//Instantiate message based on structure
	plane_data_t msg;

	//Create receive ID
	int rcvid;

	//To create a channel, and attach it to server : name_attach_t =
	//name_attach(dispatch_t * dpp, const char * path, unsigned flags);
	//Path is the name of the channel
	if ((name_attach_t = name_attach(NULL, ATTACH_POINT, 0)) == NULL){
			perror("Error occurred during creation of channel");
	}

	//To receive a message from the client
	//int MsgReceive(int chid, void * msg, size_t size, struct _msg_info * info);
	//chid is the channel ID of the server
	//*msg is the pointer to the variable where the message will be stored
	//size is the size in bytes of msg
	//*info is a NULL, or a pointer to a msg_info structure
	//Do MsgReceive here with the chid in a while(true) loop
	while(true){
		//Server will block until it receives message from client through
		//a channel named "name_attach_t" which was created with name_attach(..)
		//The received message will be stored in msg and rcvid
		rcvid = MsgReceive(name_attach_t->chid, &msg, sizeof(msg), NULL);
		//In case of error
		if(rcvid == -1) {
			perror("Error receiving message");
			break;
		}

		//Else, if properly received message/pulse
		//Pulse signal actions depending on code
		if (rcvid == 0) {/* Pulse received */
					switch (msg.hdr.code) {
					case _PULSE_CODE_DISCONNECT:
						/*
						 * A client disconnected all its connections (called
						 * name_close() for each name_open() of our name) or
						 * terminated
						 */
						ConnectDetach(msg.hdr.scoid);
						break;
					case _PULSE_CODE_UNBLOCK:
						/*
						 * REPLY blocked client wants to unblock (was hit by
						 * a signal or timed out).  It's up to you if you
						 * reply now or later.
						 */
						break;
					default:
						/*
						 * A pulse sent by one of your processes or a
						 * _PULSE_CODE_COIDDEATH or _PULSE_CODE_THREADDEATH
						 * from the kernel?
						 */
						break;
					}
					continue;
				}

		//name_open() sends a connect message, must EOK this */
		if (msg.hdr.type == _IO_CONNECT ) {
			MsgReply( rcvid, EOK, NULL, 0 );
			continue;
		}

		/* Some other QNX IO message was received; reject it */
		if (msg.hdr.type > _IO_BASE && msg.hdr.type <= _IO_MAX ) {
					MsgError( rcvid, ENOSYS );
					continue;
			}

	//Reply to client error case
	//Use this if message received not in expected format
	//int MsgError(int rcvid, int error)
	//rcvid is the output of receive function
	//error can be set to -1, ENOSYS, ERESTART, EOK, or the error code that you want to set for the client
	//First check if msg is what we expect
	//Then you can have a switch that checks subtype of message (like 0x01 for speed, 0x02 for temperature, etc)
	//Build GUI output based on the msg subtypes
	//0x00 will be for data
	if (msg.hdr.type == 0x00){
		//Motion message received
		//0x01 is subtype for velocity
		if (msg.hdr.subtype == 0x01){
			//printf("The velocity of the plane is  %.2f m/s \n", data);
			if (msg.ArrivedYet == 1){
			if (msg.outsideAirspace == 1) {
				cout << "Plane " << msg.id << " outside of airspace!" << endl;
			}
			else {
			cout << "Velocity received from plane thread (Plane ID " <<  msg.id << "): (" << msg.velocity[0] << ", " << msg.velocity[1] << ", " << msg.velocity[2] << ")" << endl;
			cout << "Position received from plane thread (Plane ID " <<  msg.id << "): (" << msg.position[0] << ", " << msg.position[1] << ", " << msg.position[2] << ")" << endl;
			}
			}
			else {
				cout << "Plane " << msg.id << " hasn't arrived yet!" << endl;
			}
		}
		else if (msg.hdr.subtype == 0x02){
			//printf("The velocity of the plane is  %.2f m/s \n", data);
			cout << "Position received from plane thread: (" << msg.position[0] << ", " << msg.position[1] << ", " << msg.position[2] << ")" << endl;
		}
		else{
			MsgError(rcvid, ENOSYS);
			continue;
		}
	}

	MsgReply(rcvid, EOK, 0, 0);
	}


	//Finally, to destroy a channel
	//Do int name_detach(name_attach_t * attach, unsigned flags);
	//attach is a pointer to the attach object received as output of the name_attach(…) function
	//flags are parameters that affect function’s behavior (just use 0)
	name_detach(name_attach_t, 0);

	return NULL;
}

int mainz(int argc, char*argv[]){
	//char cwd[256];
	//    if (getcwd(cwd, sizeof(cwd)) != NULL) {
	//        std::cout << "Current working directory is: " << cwd << std::endl;
	//    } else {
	//        std::perror("getcwd() error");
	//    }

	//vector<PlaneClass> planes = readPlanesFromFile("./planes.txt");
	//for (const auto& plane : planes) {
	//    cout << "ID: " << plane.getAircraftID() << endl;
	//    cout << "Arrival Time: " << plane.getArrivalTime() << endl;
	//    cout << "Position: (" << plane.getPosition(0) << ", " << plane.getPosition(1) << ", " << plane.getPosition(2) << ")" << endl;
	//    cout << "Velocity: (" << plane.getVelocity(0) << ", " << plane.getVelocity(1) << ", " << plane.getVelocity(2) << ")" << endl;
	//}

	//TimerStart(planes);


	//Declare thread
	pthread_t server_thread;
	int err_no;
	err_no = pthread_create(&server_thread, NULL, &radarserverthread, NULL);
	if (err_no != 0){
		perror("Error creating server thread! \n");
	}

	pthread_t client_thread;
	err_no = pthread_create(&client_thread, NULL, &PlaneClientThread, NULL);
	if (err_no != 0){
		perror("Error creating client thread! \n");
	}

	//Join thread to start it
	err_no = pthread_join(server_thread, NULL);
	if (err_no != 0){
		perror("Error joining server thread!");
	}

	err_no = pthread_join(client_thread, NULL);
	if (err_no != 0){
		perror("Error joining client thread!");
	}
	pthread_exit(EXIT_SUCCESS);
	return 0;
}
